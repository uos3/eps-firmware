#!/usr/bin/env python3
# serial_interface.py
"""A serial interface between the host and the test board.

This module implements the serial layer between the host
and the test board.  Information on the stream is stored
using the class TestBoardConnection.

Multi-threading is used to allow for asynchronous communications -
one threads polls for any received packets and records
acknowledgements and interrupts, the main thread sends packets
and processes any received data.  The threads communicate using flags
and a shared buffer.
"""
# General packages
import serial
import threading
import logging
import time
# UoS^3 test modules
import constants as const


# Serial interface constants
TIMEOUT_SEC = 1.0  # second
TEST_BAUD_RATE = 57600
PACKET_ID_TO_TYPE = {0x0: "RELAY_POWER", 0x1: "SET",
                     0x2: "MEASURE", 0x3: "UNKNOWN",
                     0x80: "RELAY_HOST", 0x81: "RELAY_INT", 0x82: "DATA",
                     0x83: "OCP", 0xFE: "ERROR", 0xFF: "ACK"}
# Use the packet type dicts but swap the values and keys, so we get a
# dict of header types and their corresponding values
PACKET_TYPE_TO_ID = dict((value, key) for
                         key, value in PACKET_ID_TO_TYPE.items())
PACKET_LENGTHS = {"RELAY_INT": 0, "DATA": 16, "OCP": 4, "ACK": 0,
                  "RELAY_POWER": 5, "SET": 9, "MEASURE:": 0, "ERROR": 1}


class TestBoardConnection:
    """Hold the serial port object and the state of this connection.

    Attributes:
            connected (bool): True if the serial connection is
                active, and the Rx thread is running.  When the
                Rx thread is killed, this is set to False and
                the program may exit.
            kill (bool): Default False, set to True if you want
                to kill the Rx thread and close the connection.
            serial_port (Serial): An object representing the serial
                stream.
            port_access (Lock): A lock which should be acquired
                before reading or writing to the serial stream.
                This ensures thread-safety across the Rx and
                main threads.
            packet_ack (bool): True if the last sent packet has been
                acknowledged by the test board MCU.
            ocp_tripped (bool): True if the OCP has been tripped since
                this flag was last reset.
            data_received (bool): True if all data has been received
                and added to the buffer.
            interrupt (bool): True if an interrupt has been triggered
                by the power supply, False otherwise.
            ocp_timer (int): The value of the OCP timer on the test
                board, note this will be None if the OCP hasn't
                tripped since last being reset.
            register_id (int): The register ID of the first received
                power supply register.
            register_count (int): The total number of registers read
                from the power supply.
            data_buffer (list): A list of bytes containing any received
                data e.g. a set of voltage measurements.
            supply_buffer (list): A list of bytes received from the
                power supply.
    """
    def __init__(self, port_name):
        """Create the serial connection and initialise state.

        Args:
            port_name (str): The name of the COM port.

        Examples:
            >>> from serial_interface import TestBoardConnection
            >>> test_board_connection = TestBoardConnection("COM1")
        """
        self.connected = True
        self.kill = False
        # Open the connection
        self.serial_port = serial.Serial(port=port_name,
                                         baudrate=TEST_BAUD_RATE,
                                         timeout=TIMEOUT_SEC)
        self.port_access = threading.Lock()

        # Flags
        self.packet_ack = False
        self.ocp_tripped = False
        self.data_received = False
        self.interrupt = False

        # Data, set any invalid variables to None
        self.ocp_timer = None
        self.register_id = None
        self.register_count = 0
        self.data_buffer = []
        self.supply_buffer = []

    def close(self):
        """Close the serial connection and kill any threads."""
        self.kill = True
        logging.debug("Closing serial connection.")
        while self.connected:
            pass
        self.serial_port.close()

    def clear_data_buffer(self):
        """Clear the data buffer."""
        self.data_buffer = []
        self.data_received = False


def open_connection(port_name):
    """Open a serial port and associate it with a state.

    Take a port name e.g. 'COM1' and attempt to open this port with
    a bit-rate of 57600.  Associate this connection with an object
    which stores information on the state of the stream e.g. has the
    last sent packet been acknowledged by the test MCU?  Start polling
    for received packets in a separate thread - this was done because
    communications are asynchronous between the host and test board.

    Args:
        port_name (str): The name of the COM port to open.

    Returns:
        TestBoardConnection: An object containing the serial
            connection and its state.

    Examples:
        >>> from serial_interface import open_connection
        >>> new_connection = open_connection("COM1")
    """

    connection = TestBoardConnection(port_name)
    logging.info("Connection opened on {}".format(port_name))
    # Start a thread to constantly poll for received packets; communications
    # are asynchronous between the host and test board.
    rx_thread = threading.Thread(target=_poll_rx, args=(connection,))
    rx_thread.start()

    return connection


def send_packet(board_connection, header, payload=None):
    """Send a packet to the test MCU.

    Args:
        board_connection (TestBoardConnection): An object containing
            the serial connection and its state.
        header (str): The type of packet to send e.g. 'MEASURE'.  This
            must be a valid type - 'RELAY_POWER', 'SET' or 'MEASURE'.
        payload (list): The payload bytes e.g. the message to relay to
            the power supply [default None].

    Raises:
        ValueError: The header must be a valid packet type.
        TimeoutError: The serial port will timeout if the expected
            data isn't received within the timeout limit.

    Examples:
        >>> import serial_interface
        >>> board_connection = serial_interface.open_connection("COM1")
        >>> send_packet(board_connection, "MEASURE")
    """
    board_connection.packet_ack = False

    # Get the header byte from the type, check that the type is recognised
    packet_id = PACKET_TYPE_TO_ID.get(header.upper())
    if packet_id is None:
        raise ValueError("Packet type {} not recognised".format(header))

    # Create the packet from the header and the payload
    packet = [packet_id]
    if payload:
        packet.append(len(payload))
        packet += payload
    else:
        packet.append(0)

    with board_connection.port_access:
        board_connection.serial_port.write(packet)

    # Wait for an acknowledgement
    time_elapsed = 0.0
    while not board_connection.packet_ack and time_elapsed < TIMEOUT_SEC:
        time.sleep(1.0E-3)
        time_elapsed += 1.0E-3

    if not board_connection.packet_ack:
        raise(TimeoutError("Acknowledgement timed out."))
    else:
        logging.info("{} sent and acknowledged.".format(header.upper()))
        logging.debug("Packet Contents: {}".format(packet))


def receive_packet(board_connection, header_byte):
    """Receive a packet from the test board.

    Read the expected number of bytes from the serial stream -
    the packet header indicates the type of packet therefore
    the expected length.  Do some basic processing to flag
    events such as OCP being tripped to the main thread.  Any
    data is stored in a buffer which is copied over by the
    main thread later.

    Args:
        board_connection (TestBoardConnection): An object containing
            the serial connection and its state.
        header (Byte): The header byte.

    Raises:
        ValueError: The header must be a valid packet type.
        TimeoutError: The serial port will timeout if the expected
            data isn't received within the timeout limit.
    """
    packet_id = ord(header_byte)
    # Do a sanity check on the header
    packet_type = PACKET_ID_TO_TYPE.get(packet_id)
    if packet_type is None:
        raise ValueError("Header {} not recognised".format(packet_id))
    logging.info("{} packet received.".format(packet_type))

    # If the header is okay, get the length of the remaining packet which
    # is transmitted in the 2nd byte
    with board_connection.port_access:
        length_byte = board_connection.serial_port.read()
    if len(length_byte) < 1:
        raise TimeoutError("Packet length byte not received.")

    payload_length = ord(length_byte)
    packet = [packet_id, payload_length]

    # Read the payload, checking that the stream doesn't timeout
    if payload_length > 0:
        logging.debug("Payload is {} bytes.".format(payload_length))
        with board_connection.port_access:
            payload = board_connection.serial_port.read(payload_length)
        # Check all bytes are received i.e. the connection didn't time out
        if len(payload) < payload_length:
            raise TimeoutError("Expected packet data not received.")
        packet += payload

    # Use a dict of functions to associate different packet types with the
    # functions that process them
    process_packet = {"RELAY_HOST": _relay_host, "RELAY_INT": _relay_interrupt,
                      "DATA": _data, "OCP": _ocp, "ACK": _ack,
                      "ERROR": _error}
    # Process the received packet, generally this will modify the state
    # of the connection object or update a data buffer for the main
    # thread to access
    process_packet[packet_type](board_connection, packet)

    return packet


def crc_16(packet_bytes):
    """Perform a cyclic redundancy check (CRC) on a list of bytes.

    Use CRC-16, with the polynomial 1 + x^2 + x^15 + x^16 according
    to the document AD-03, Appendix C - LRC/CRC Generation, and the
    Communication Protocol Specification for the UoS^3 project.

    Args:
        packet_bytes (list): The packet to generate a CRC from.

    Returns:
        Int: The 16-bit value of the CRC register.

    Examples:
        >>> from serial_interface import crc_16
        >>> crc_16((0x0, 0x3, 0x4))
        62320
    """
    crc_register = 0xFFFF
    crc_polynomial = 0xA001

    for curr_byte in packet_bytes:
        crc_register ^= curr_byte
        for i in range(0, 8):
            # Take the LSB and shift the register left
            lsb = crc_register & 1
            crc_register >>= 1
            if lsb:
                crc_register ^= crc_polynomial
            # else do nothing and shift left

    return crc_register


def int_to_bytes(value, byte_count=2):
    """Get the byte representation of a an integer.

    All multiple byte values are transmitted using a little
    endian representation in the communication protocol.

    Args:
        value (int): An integer value.
        byte_count (int): The size of the integer in bytes [default
            2].

    Returns:
        List: A list of bytes, starting with the least significant
            byte.

    Examples:
        >>> from serial_interface import int_to_bytes
        >>> int_to_bytes(52651)
        [171, 205]

    """
    byte_representation = []
    for i in range(0, byte_count):
        byte_representation.append(value & 0xFF)
        value >>= 8

    return byte_representation


def bytes_to_int(byte_representation):
    """Get the integer value from a byte representation.

    All multiple byte values are transmitted little endian.

    Args:
        byte_representation (list): A list of bytes.

    Returns:
        Int: The integer value represented by the bytes.

    Examples:
        >>> from serial_interface import bytes_to_int
        >>> bytes_to_int((0xAB, 0xCD))
        52651
    """
    value = 0
    for curr_byte in byte_representation[::-1]:
        # Append each byte, starting with the last byte (most significant)
        value <<= 8
        value += curr_byte

    return value


def _poll_rx(board_connection):
    # Poll the serial COM port for any received packets.  This will likely be
    # done in a separate thread to main execution.  If something is received,
    # wait on the rest of the packet.
    while not board_connection.kill:
        # If bytes are available to read on the serial port, then read the
        # first byte from the stream
        if board_connection.serial_port.inWaiting() > 0:
            with board_connection.port_access:
                header = board_connection.serial_port.read()

            if not len(header):
                raise(serial.SerialException("Failed to read header byte."))
            try:
                packet = [header]
                packet += receive_packet(board_connection, header)
            except (TimeoutError, ValueError) as rx_exception:
                logging.exception(rx_exception)
            else:
                logging.debug("Received bytes: {}".format(packet))
        # else do nothing, no packets have been received yet

    # When exiting, flag the connection as being killed successfully
    board_connection.connected = False


def _relay_host(board_connection, packet):
    # Process a packet relayed from the power supply MCU to the host. A relayed
    # packet from the host contains the values read from a requested range of
    # registers.  The first payload byte is the starting register ID,
    # and the second payload byte says how many registers to read.  2*N
    # bytes contain N register values, followed by a CRC.  The power supply
    # message may be split across multiple packets.
    message_len = None
    relayed_data = packet[2:]

    for byte in relayed_data:
        board_connection.supply_buffer.append(byte)

        if len(board_connection.supply_buffer) >= 2:
            # If the message length hasn't been calculated yet, use the number
            # of registers read / written to calculate the message length -
            # telling us how many more bytes need to be received
            if message_len is None:
                register_count = board_connection.supply_buffer[1]
                message_len = (2 * register_count) + 4  # bytes

            # If an entire message has been received from the power supply,
            # process it
            if len(board_connection.supply_buffer) >= message_len:
                _process_relayed_message(board_connection)


def _ack(board_connection, _):
    # Record an acknowledgement packet from the test board
    logging.debug("Last sent packet acknowledged by the test board.")
    board_connection.packet_ack = True


def _ocp(board_connection, packet):
    # Record an OCP packet from the test board
    logging.debug("OCP details received from the test board.")
    board_connection.ocp_tripped = True
    board_connection.ocp_timer = bytes_to_int(packet[2:])
    print("OCP Tripped: {}".format(board_connection.ocp_timer))  # DEBUG


def _data(board_connection, packet):
    # Receive test board voltage measurements, store the received data in a
    # buffer for the main thread
    logging.debug("Measurements received from the test board.")
    board_connection.data_buffer = packet[2:]
    board_connection.data_received = True


def _error(_, packet):
    # Record an Rx error from the test board.  An error packet indicates
    # something went wrong between the host and test board e.g. the test board
    # received an invalid packet ID.
    error_code = packet[2]
    # TODO: parse the error code
    logging.warning("Test MCU error received (code: {})".format(error_code))


def _relay_interrupt(board_connection, _):
    # Record a power supply MCU interrupt.
    logging.debug("Interrupt received from power supply.")
    board_connection.interrupt = True


def _process_relayed_message(board_connection):
    # Process a message relayed from the power supply
    message = board_connection.supply_buffer.copy()
    # Clear the power supply message buffer
    board_connection.clear_data_buffer()
    # Perform the CRC on the payload and compare against the CRC bytes
    message_crc = bytes_to_int(message[-2:])
    generated_crc = crc_16(message[:-2])

    logging.debug("Received message bytes: {}".format(message))
    logging.debug("CRC generated: 0x{0:x}, "
                  "CRC received: 0x{1:x}".format(
                   generated_crc, message_crc))

    # Perform a CRC
    if generated_crc != message_crc:
        raise(ValueError("CRC check failed: "
                         "CRC generated: 0x{0:x}, "
                         "CRC received: 0x{1:x}".format(
                          generated_crc, message_crc)))

    # Store the register values in a buffer, ignoring any header or CRC
    # bytes.  This buffer will be read by the main thread and used to
    # update the registers.
    board_connection.register_id = message[0]
    board_connection.register_count = message[1]

    register_values = [bytes_to_int(message[i:i+2])
                       for i in range(2, len(message)-2, 2)]
    board_connection.data_buffer = register_values
    board_connection.data_received = True
    board_connection.supply_buffer = []


if __name__ == "__main__":
    # If called from the command-line, demonstrate the serial connection
    # works by opening and closing the connection
    import sys

    logging.basicConfig(filename="serial_interface.log",
                        format=const.LOG_FORMAT,
                        filemode='w', level=logging.DEBUG)

    if len(sys.argv) < 2:
        print("Usage: serial_interface.py [COM port]")
    else:
        try:
            print("Opening connection on serial port.")
            serial_connection = open_connection(sys.argv[1])
        except TimeoutError as serial_exception:
            logging.exception(serial_exception)
        else:
            print("Connection opened successfully, now closing connection.")
            serial_connection.close()
            print("Connection closed, exiting.")
