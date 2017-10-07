#!/usr/bin/env python3
# virtual_board.py
"""Emulate the test board for debugging.

Use a virtual serial connection to emulate the test board on the
host.  This is useful for debugging other modules in this package
without a need for hardware.

Note that the virtual serial connection must be
provided by a program such as 'com0com', or 'Virtual Serial Ports'.
"""
# General packages
import serial
import random
import sys
# UoS^3 test modules
import serial_interface

from constants import TEST_BAUD_RATE
from constants import PACKET_ID_TO_TYPE
from constants import PACKET_TYPE_TO_ID
from constants import REGISTER_COUNT
from constants import REGISTER_ID
from constants import MIN_TRIP_CURRENT
from constants import CRC_BIT
from constants import VOLTAGE_DAC_ADDRESS

supply_voltage = 0
current_drawn = [0, 0, 0, 0, 0, 0, 0, 0]
line_voltages = [255, 255, 255, 255, 255, 255, 255, 255]
registers = [0 for i in range(0, REGISTER_COUNT)]
VIRTUAL_TIMEOUT = 1.0E-4  # seconds


def run(serial_port):
    """Emulate the test board by monitoring a virtual COM port."""
    try:
        while True:
            if serial_port.inWaiting() > 0:
                header = serial_port.read()
                # Check sanity of header byte
                if not len(header):
                    raise serial.SerialException("Failed to read header.")

                # When a packet is received
                packet_type = PACKET_ID_TO_TYPE.get(ord(header), "UNKNOWN")
                print("{0} packet received ({1})".format(str(packet_type),
                                                         str(header)))
                payload = read_packet(serial_port)
                # First acknowledge the packet has been received
                _ack(serial_port)
                # Then respond to the message
                response = {"MEASURE": _measure, "SET": _set_board,
                            "RELAY_POWER": _relay, "UNKNOWN": _unknown}
                response[packet_type](serial_port, payload)
                print("Test board has processed host message.")

    except KeyboardInterrupt:
        print("Program stopped using keyboard interrupt, exiting.")


def read_packet(serial_port):
    """Read a packet from the serial port."""
    length_byte = serial_port.read(1)
    payload_length = ord(length_byte)
    payload = None

    if payload_length > 0:
        print("Reading payload of {} bytes.".format(payload_length))
        payload = serial_port.read(payload_length)
        if len(payload) < payload_length:
            print("Warning: Fewer payload bytes received than expected.")

    return payload


def interrupt_triggered():
    """Return True if the interrupt line is triggered (high).

    Returns:
        Bool: True if the interrupt line is high, False otherwise.
    """
    global registers

    config = registers[REGISTER_ID["CONFIG"]]
    status = registers[REGISTER_ID["STATUS"]]

    return (~config) & status


def _set_board(serial_port, payload):
    # Respond to a SET message.  The test board will set its voltages and
    # current drawn.  No response is needed - simply update the values of
    # the supply voltage and current drawn.
    global current_drawn
    global supply_voltage
    global registers

    rail_address = payload[0]
    rail_value = payload[1]

    if rail_address < VOLTAGE_DAC_ADDRESS:
        current_drawn[rail_address] = rail_value
        # If the current drawn exceeds the current limit for that rail,
        # trip the virtual OCP
        if current_drawn[rail_address] > MIN_TRIP_CURRENT[rail_address]:
            prev_int_status = interrupt_triggered()
            registers[REGISTER_ID["STATUS"]] |= (1 << rail_address)
            # If interrupt line has transitioned from low to high, make
            # the host aware with an RELAY_INT packet
            if not prev_int_status and interrupt_triggered():
                int_packet_length = 0
                serial_port.write((PACKET_TYPE_TO_ID["RELAY_INT"],
                                   int_packet_length))
    else:
        supply_voltage = rail_value

    print("Rail {0} set to {1}".format(rail_address, rail_value))


def _relay(serial_port, payload, error_rate=0):
    # Respond to a RELAY_POWER message, responding by either providing
    # the requested registers or writing to the specified registers.
    global registers
    print(registers)

    write = payload[0] & 0b10000000
    start_register_id = payload[0] & 0b01111111
    register_count = payload[1]

    # Convert to a 'bytearray' so we can modify the individual bytes
    payload = bytearray(payload)
    # Model packet errors by modifying each byte with probability
    # 'error_rate'
    for i in range(0, len(payload)):
        if random.random() < error_rate:
            # Introduce a random error
            random_value = random.randint(0, 0xFF)
            print("Introducing an error, byte changed from {0} to {1}".format(
                payload[i], random_value))
            payload[i] = random_value
        # else do nothing i.e. no error is introduced

    # Check the CRC, if the CRC fails the packet is invalid
    packet_crc = serial_interface.bytes_to_int(payload[-2:])
    generated_crc = serial_interface.crc_16(payload[:-2])

    if packet_crc != generated_crc:
        print("Error: CRC mismatch ({0:x} from the host,"
              "{1:x} generated".format(packet_crc, generated_crc))
        # Set the CRC fail bit in the status register
        prev_int_status = interrupt_triggered()
        registers[REGISTER_ID["STATUS"]] |= CRC_BIT
        if not prev_int_status and interrupt_triggered():
            # If interrupt line has transitioned from low to high, make
            # the host aware with an RELAY_INT packet
            int_packet_length = 0
            print("Relaying interrupt from power supply to host.")
            serial_port.write([PACKET_TYPE_TO_ID["RELAY_INT"],
                               int_packet_length])
    elif write:
        print("Writing new values to registers {0}-{1}".format(
            start_register_id, start_register_id+register_count))
        for i in range(0, register_count):
            registers[start_register_id+i] = payload[2+2*i]
            registers[start_register_id+i] += payload[3+(2*i)] << 8
        # No response is required
    else:  # if a read
        print("Reading registers {0}-{1}".format(
            start_register_id, start_register_id+register_count))
        packet_length = 4 + (2*register_count)
        # Start with the packet header, length and the first register ID,
        # finally with the number of registers read, stored little-endian
        packet = [PACKET_TYPE_TO_ID["RELAY_HOST"], packet_length,
                  start_register_id, register_count]
        # Get the value of all requested registers
        for curr_register in registers[start_register_id:
                                       start_register_id+register_count]:
            packet += serial_interface.int_to_bytes(curr_register)
        # Add the CRC, stored little-endian
        crc = serial_interface.crc_16(packet[2:])
        packet += serial_interface.int_to_bytes(crc)
        # Send the packet to the host
        serial_port.write(packet)


def _ack(serial_port):
    # Send an acknowledgement packet.
    packet_length = 0
    serial_port.write((PACKET_TYPE_TO_ID["ACK"], packet_length))
    print("Packet acknowledged.")


def _measure(serial_port, _):
    global line_voltages

    packet = [PACKET_TYPE_TO_ID["MEASUREMENT"], 8]
    packet += line_voltages
    serial_port.write(packet)
    print("Voltage measurements sent to the host.")


def _unknown(*_):
    # A placeholder function for receiving invalid packets.
    print("Warning: Packet type unknown.")


if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: virtual_board.py [COM port]")
    else:
        print("Opening virtual serial port on port {}".format(sys.argv[1]))
        print("Use Ctrl+C or Ctrl+D to stop this script.")
        virtual_port = serial.Serial(port=sys.argv[1], baudrate=TEST_BAUD_RATE,
                                     timeout=VIRTUAL_TIMEOUT)
        run(virtual_port)
