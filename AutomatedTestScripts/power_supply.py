# power_supply.py
"""Functions to control and read from the power supply.

The power supply manages a set of registers which may be read and
written to via relayed messages.  This module provides functions
to read and write these registers.  There are also higher level
functions which perform useful tasks e.g. enabling / disabling a
specific rail or requesting battery telemetry.
"""
import serial_interface
import time
import logging


REGISTER_COUNT = 0x32
SETTLE_TIME = 1.0E-3  # seconds
DEFAULT_MAX_ATTEMPTS = 3  # attempts at sending a packet until we give up
CRC_BIT = 0b100000000  # 2^8
OCP_BITS = 0b00111111
REG_WRITE_BIT = 0b10000000
CRC_MIN_WAIT = 1.0E-2  # seconds
REGISTER_ID = {"CONFIG": 0x00, "STATUS": 0x01, "SW_OFF": 0x02, "POWER": 0x03,
               "BAT_V": 0x04, "BAT_I": 0x05, "BAT_T": 0x06,
               "SOLAR_N1_I": 0x07, "SOLAR_N2_I": 0x08,
               "SOLAR_S1_I": 0x09, "SOLAR_S2_I": 0x0A,
               "SOLAR_E1_I": 0x0B, "SOLAR_E2_I": 0x0C,
               "SOLAR_W1_I": 0x0D, "SOLAR_W2_I": 0x0E,
               "SOLAR_T1_I": 0x0F, "SOLAR_T2_I": 0x10,
               "SOLAR_2_V": 0x11,  # 0x12 is Reserved
               "RAIL1_BOOT": 0x13, "RAIL1_FAIL": 0x14,
               "RAIL1_V": 0x15, "RAIL1_I": 0x16,
               "RAIL2_BOOT": 0x17, "RAIL2_FAIL": 0x18,
               "RAIL2_V": 0x19, "RAIL2_I": 0x1A,
               "RAIL3_BOOT": 0x1B, "RAIL3_FAIL": 0x1C,
               "RAIL3_V": 0x1D, "RAIL3_I": 0x1E,
               "RAIL4_BOOT": 0x1F, "RAIL4_FAIL": 0x20,
               "RAIL4_V": 0x21, "RAIL4_I": 0x22,
               "RAIL5_BOOT": 0x23, "RAIL5_FAIL": 0x24,
               "RAIL5_V": 0x25, "RAIL5_I": 0x26,
               "RAIL6_BOOT": 0x27, "RAIL6_FAIL": 0x28,
               "RAIL6_V": 0x29, "RAIL6_I": 0x2A,
               "SUPPLY_3_V": 0x2B, "SUPPLY_3_I": 0x2C,
               "SUPPLY_5_V": 0x2D, "SUPPLY_5_I": 0x2E,
               "CHARGE_I": 0x2F, "MPPT_BUS_V": 0x30,
               "CRC_ER_CNT": 0x31, "DROP_CNT": 0x32}

CONVERSION_FACTOR = {"BAT_V": 10.0 / 41.0,
                     "SOLAR_N1_I": 4.2, "SOLAR_N2_I": 4.2,
                     "SOLAR_S1_I": 4.2, "SOLAR_S2_I": 4.2,
                     "SOLAR_E1_I": 4.2, "SOLAR_E2_I": 4.2,
                     "SOLAR_W1_I": 4.2, "SOLAR_W2_I": 4.2,
                     "SOLAR_T1_I": 4.2, "SOLAR_T2_I": 4.2,
                     "SOLAR_2_V": 10.0 / 41.0,
                     "RAIL1_V": 10.0 / 41.0, "RAIL1_I": 25.0,
                     "RAIL2_V": 10.0 / 41.0, "RAIL2_I": 3.8,
                     "RAIL3_V": 10.0 / 41.0, "RAIL3_I": 11.4,
                     "RAIL4_V": 10.0 / 41.0, "RAIL4_I": 45.0,
                     "RAIL5_V": 10.0 / 41.0, "RAIL5_I": 4.5,
                     "RAIL6_V": 10.0 / 41.0, "RAIL6_I": 11.4,
                     "SUPPLY_3_V": 10.0 / 41.0, "SUPPLY_3_I": 25.0,
                     "SUPPLY_5_V": 10.0 / 41.0, "SUPPLY_5_I": 12.5,
                     "CHARGE_I": 25.0, "MPPT_BUS_V": 10.0 / 41.0}

# Convert from test board rail number to power supply rail number, this was
# different because I followed the power subsystem rail numbers on the
# test board - these numbers were not used for the supply PCB layout
TEST_BOARD_TO_SUPPLY_RAIL = (4, 3, 5, 6, 2, 1, 8, 7)


def check_connection(board_connection):
    """Check the connection between the test board and the power supply.

    Returns:
        Bool: True if the power supply responded, False otherwise.
    """
    # Create a set of dummy registers - these values will be
    # discarded
    registers = [0 for _ in range(0, REGISTER_COUNT)]

    try:
        request_registers(board_connection, registers, 0x0, 1)
    except TimeoutError:
        passed = False
    else:
        passed = True

    return passed


def initialise(board_connection):
    """Initialise the power supply.

    """
    logging.debug("Initialising power supply.")
    registers = initialise_registers(board_connection)
    battery = request_battery_measurements(board_connection, registers)

    for rail in range(1, 7):
        change_rail_status(board_connection, registers,
                           rail, enabled=False)

    return registers, battery


def initialise_registers(board_connection):
    """Create and initialise a list to store the registers values.

    Request all register values from the power supply and initialise
    a list with these values.  This host will monitor communications
    and update certain registers e.g. when querying the state of
    the OCP.

    Args:
        board_connection (TestBoardConnection): An object containing
            the connection stream and its state.
        registers (list): The last read power supply register values.

    Returns:
        List: A list containing all the register values.
    """
    registers = [0 for _ in range(0, REGISTER_COUNT)]

    logging.debug("Initialising registers to power supply values.")
    # Request the value of all registers
    request_registers(board_connection, registers, 0x0, REGISTER_COUNT,
                      verify_crc=True)

    return registers


def request_registers(board_connection, registers, first_register_id,
                      register_count, verify_crc=False):
    """Read the specified power supply registers.

    Args:
        board_connection (TestBoardConnection): An object containing
            the connection stream and its state.
        registers (list): The last read power supply register values.
        first_register_id (int): The address of the first register
            to read from the power supply MCU.
        register_count (int): The number of registers to read.
        verify_crc (bool): If True check that the request CRC is
            correct [default False].

    Returns:
        List: A list of the updated register values, ordered
            sequentially from the lowest to highest address.
    """
    if register_count > 1:
        register_str = "Requesting the values of registers {0:x}-{1:x}".format(
            first_register_id, first_register_id+register_count-1)
    else:
        register_str = "Requesting the values of register {0:x}".format(
            first_register_id, first_register_id+register_count-1)
    logging.debug(register_str)
    board_connection.clear_data_buffer()
    # Create the packet to relay to the power supply MCU
    payload = [first_register_id, register_count, 0x0]

    # Append the two byte CRC, stored little-endian
    generated_crc = serial_interface.crc_16(payload)
    # Split 16-bit value into the low and high bytes
    payload += serial_interface.int_to_bytes(generated_crc)
    # Send the packet to the power supply MCU
    _relay_packet(board_connection, registers, payload, verify_crc)

    # Wait for the power supply to respond
    time_elapsed = 0.0
    while (not board_connection.data_received and
           time_elapsed < serial_interface.TIMEOUT_SEC):
        time.sleep(1.0E-3)
        time_elapsed += 1.0E-3

    if not board_connection.data_received:
        raise(TimeoutError("Timed out waiting for PSU MCU register values"))

    # Update the registers with their new values
    updated = board_connection.data_buffer.copy()
    registers[first_register_id:first_register_id+register_count] = updated
    board_connection.clear_data_buffer()
    board_connection.register_id = None
    board_connection.register_count = 0
    logging.info("Registers {}-{} updated.".format(
        first_register_id, first_register_id+register_count-1))

    return updated


def write_register(board_connection, registers, register_id, write,
                   verify_crc=False):
    """Write a new value to a power supply register.

    Args:
        board_connection (TestBoardConnection): An object containing
            the connection stream and its state.
        registers (list): The last read power supply register values.
        register_id (int): The register address (0x0-0x2D).
        write (int): A 16-bit values to write into the specified
            register.
        verify_crc (bool): If True check that the write CRC is
            correct [default False].
    """
    logging.debug("Writing value {0} to register {1:x}".format(write,
                                                               register_id))
    # Indicate a write by setting the most significant bit to '1'
    relay_power_header = REG_WRITE_BIT | register_id
    payload = [relay_power_header] + serial_interface.int_to_bytes(write)

    # Append the two CRC bytes and send
    crc = serial_interface.crc_16(payload)
    payload += serial_interface.int_to_bytes(crc)
    _relay_packet(board_connection, registers, payload, verify_crc)

    # Wait for the power supply to respond to the register write, the PSU
    # echoes back the new register values
    time_elapsed = 0.0
    while (not board_connection.data_received and
           time_elapsed < serial_interface.TIMEOUT_SEC):
        time.sleep(1.0E-3)
        time_elapsed += 1.0E-3

    if board_connection.data_received:
        # Flush the buffer
        board_connection.clear_data_buffer()
    else:
        logging.warning("Timed out waiting for PSU MCU response.")
        #raise(TimeoutError("Timed out waiting for PSU MCU response."))


def change_rail_status(board_connection, registers, test_board_rail, enabled):
    """Enable or disable a given rail.

    All eight supply rails can be enabled or disabled by the power
    supply MCU.  Relay a packet to the power supply to select
    a given rail and put it in the given mode.

    Args:
        board_connection (TestBoardConnection): An object containing
            the connection stream and its state.
        registers (list): The power supply registers.
        test_board_rail (int): The test board rail number (1-8).
        enabled (bool): True to enable, False to disable the rail.
    """
    # Unfortunately the test board rail numbers don't match the power supply
    # rail numbers, blame me... map to the correct rail number
    supply_rail = TEST_BOARD_TO_SUPPLY_RAIL[test_board_rail-1]
    # Update the SW_OFF register
    request_registers(board_connection, registers,
                      REGISTER_ID["SW_OFF"], 1)
    sw_off = registers[REGISTER_ID["SW_OFF"]]
    # Toggle the requested rail by modifying the bit-field corresponding
    # to the given rail
    if enabled:
        logging.info("Enabling supply rail {}".format(supply_rail))
        sw_off |= (1 << (supply_rail-1))
    else:  # if disabled
        logging.info("Disabling supply rail {}".format(supply_rail))
        sw_off &= ~(1 << (supply_rail-1))
    
    write_register(board_connection, registers, REGISTER_ID["SW_OFF"],
                   sw_off, verify_crc=True)


def request_battery_measurements(board_connection, registers):
    """Request battery telemetry from the power supply.

    The battery voltage, current and temperature is available from
    the power supply MCU.  This is taken from the battery board.

    Args:
        board_connection (TestBoardConnection): An object containing
            the connection stream and its state.
        registers (list): The last read power supply register values.
    """
    logging.debug("Requesting battery telemetry")
    battery_address = REGISTER_ID["BAT_V"]
    register_count = 3

    # Update the voltage, current and temperature register values
    battery_registers = request_registers(board_connection,
                                          registers, battery_address,
                                          register_count, verify_crc=True)

    battery_voltage = adc_to_measurement(battery_registers[0], "BAT_V")
    battery_current = adc_to_measurement(battery_registers[1], "CHARGE_I")

    return battery_voltage, battery_current


def request_rail_measurements(board_connection, registers, rail):
    """Request measurements from the power supply for a given rail.

    The power supply provides power to eight different rails, with
    capability to measure voltage and current on each line.

    Args:
        board_connection (TestBoardConnection): An object containing
            the connection stream and its state.
        registers (list): The last read power supply register values.
        rail (int): The rail number (1-8).

    Returns:
        Int: The successful boot count.
        Int: The failed boot count.
        Float: The measured voltage.
        Float: The measured current.

    Raises:
        ValueError: The rail number  must be  be > 0 and <= 8.
    """
    logging.debug("Requesting voltage measurement on rail {}".format(rail))

    # Check rail number is valid
    if rail < 1 or rail > 8:
        raise ValueError("Rail number must be > 0 and <= 8.")

    supply_rail = TEST_BOARD_TO_SUPPLY_RAIL[rail-1]
    register_names = ("RAIL1", "RAIL2", "RAIL3", "RAIL4", "RAIL5",
                      "RAIL6", "SUPPLY_3", "SUPPLY_5")

    # Get the needed registers from the power MCU
    if supply_rail <= 6:
        register_count = 4
        rail_register_id = 0x13 + (register_count * (supply_rail-1))
        measurements = request_registers(board_connection, registers,
                                         rail_register_id, register_count=4,
                                         verify_crc=False)
        boots = measurements[0]
        fails = measurements[1]
        # Convert from the DAC and values to a voltage / current
        rail_name = register_names[supply_rail-1]
        voltage = (adc_to_measurement(measurements[2], rail_name+"_V"))
        current = (adc_to_measurement(measurements[3], rail_name+"_I"))

        return boots, fails, voltage, current
    else:
        register_count = 2
        rail_register_id = 0x2B + (register_count * (supply_rail-7))
        measurements = request_registers(board_connection, registers,
                                         rail_register_id, register_count=4,
                                         verify_crc=False)
        rail_name = register_names[supply_rail-1]
        voltage = (adc_to_measurement(measurements[0], rail_name+"_V"))
        current = (adc_to_measurement(measurements[1], rail_name+"_I"))
        return voltage, current


def request_ocp_info(board_connection, registers):
    """Request OCP information from the power supply MCU.

    OCP data is stored within registers belonging to the power
    supply MCU.  Request the values of these registers and take
    the useful data from this.

    Args:
        board_connection (TestBoardConnection): An object containing
            the connection stream and its state.
        registers (list): The last read power supply register values.

    Returns:
        rails_tripped (list): The OCP status of each line.
        trip_time (float): The trip time in seconds.
    """

    # Update the status register, this contains information about the OCP
    request_registers(board_connection, registers,
                      REGISTER_ID["STATUS"], register_count=1,
                      verify_crc=True)

    # Clear the OCP interrupt
    clear_interrupts(board_connection, registers, ocp=True, crc=False)

    # Find which rail(s) have tripped
    status = registers[REGISTER_ID["STATUS"]]
    # The low byte of the register is a bit-mask, where the nth bit
    # corresponds to the trip status on the nth rail
    rails_tripped = [((status >> i) & 1) for i in range(0, 8)]
    clear_interrupts(board_connection, registers)

    return rails_tripped


def check_crc_status(board_connection, registers):
    """Request CRC info from the power supply MCU.

    Args:
        board_connection (TestBoardConnection): An object containing
            the connection stream and its state.
        registers (list): The last read power supply register values.

    Returns:
        Bool: True if the power supply CRC has passed, False otherwise.
    """
    if board_connection.interrupt:
        # If an interrupt has been triggered it may have been a CRC interrupt
        logging.debug("Checking power supply CRC status.")
        data_buffer = board_connection.data_buffer.copy()
        request_registers(board_connection, registers,
                          REGISTER_ID["STATUS"], register_count=1)

        # Use the CRC bit in the status register to see if it failed '1', or
        # passed '0'
        status = registers[REGISTER_ID["STATUS"]]
        crc_pass = not (status & CRC_BIT)

        # Clear the CRC interrupt flags
        clear_interrupts(board_connection, registers, ocp=False, crc=True)

        # Restore the data buffer
        board_connection.data_buffer = data_buffer
    else:  # Assume if no interrupt was flagged the CRC passed
        crc_pass = True

    return crc_pass


def clear_interrupts(board_connection, registers, ocp=True, crc=True,
                     force_write=True):
    """Clear any interrupts triggered on the power supply.

    Args:
        board_connection (TestBoardConnection): An object containing
            the connection stream and its state.
        registers (list): The last read power supply register values.
        ocp (bool): If True then clear the OCP interrupt flags.
        crc (bool): If True then clear the CRC interrupt flag.
        force_write (bool): If True, write to the power supply register
            regardless of whether the interrupt has been triggered at
            the host. If False, only bother clearing interrupts if an
            interrupt has been received - this saves sending
            redundant packets to the test board [default True].
    """
    if board_connection.interrupt or force_write:
        # Clear interrupts by writing 1 to the status register bits
        interrupt_bits = 0
        if ocp:
            logging.debug("Resetting OCP interrupt bits.")
            interrupt_bits += OCP_BITS
        if crc:
            logging.debug("Resetting CRC interrupt bits.")
            interrupt_bits += CRC_BIT

        if interrupt_bits:
            write_register(board_connection, registers,
                           REGISTER_ID["STATUS"], interrupt_bits)

        board_connection.interrupt = False


def enable_interrupt(board_connection, registers, ocp=True, crc=True):
    """Enable or disable OCP and CRC interrupts.

    Args:
        board_connection (TestBoardConnection): An object containing
            the connection stream and its state.
        registers (list): The last read power supply register values.
        ocp (bool): If True enable OCP interrupts, if False disable.
        crc (bool): If True enable the CRC interrupt, if False disable.
    """
    config = 0
    # Write to the CONFIG register, which contains a set of interrupt masks.
    # If an interrupt mask is '1', this event is ignored by the power
    # supply and won't generate an interrupt.
    if ocp:
        logging.debug("Enabling OCP interrupt.")
    else:
        config |= OCP_BITS
        logging.debug("Disabling OCP interrupt.")

    if crc:
        logging.debug("Enabling CRC interrupt.")
    else:
        config |= CRC_BIT
        logging.debug("Disabling CRC interrupt.")
    write_register(board_connection, registers, REGISTER_ID["CONFIG"],
                   config)


def adc_to_measurement(adc_value, register_name):
    """Convert an ADC value to Amps or Volts.

    This calculation was taken from the Power Supply Communication
    Specification.

    Args:
        adc_value (int): The 10-bit ADC value.
        register_name (string): Where the measurement was taken from.
    """
    measurement = (2.5 * adc_value)
    measurement /= (1023.0 * CONVERSION_FACTOR[register_name])

    return measurement


def _relay_packet(board_connection, registers, payload, verify_crc=False,
                  max_attempts=DEFAULT_MAX_ATTEMPTS):
    # Relay a packet to the power supply MCU.  If verify_crc is True, check
    # that the CRC is correct at the power supply end.  By setting
    # max_attempts, you can possibly resend a packet that fails to send.
    logging.debug("Relaying packet to the power supply.")
    if verify_crc:
        clear_interrupts(board_connection, registers, crc=True, ocp=False, force_write=True)

    curr_attempt = 1
    while curr_attempt <= max_attempts:
        # Send a write request to the power supply MCU
        serial_interface.send_packet(board_connection, "RELAY_POWER", payload)

        if verify_crc:
            # Wait a short amount of time to allow for a CRC interrupt
            # to be sent from the power supply and relayed to the host
            time.sleep(CRC_MIN_WAIT)
            crc_pass = check_crc_status(board_connection, registers)

            if crc_pass:
                logging.debug("CRC passed, packet relayed successfully.")
                break
            else:
                max_attempts += 1
                logging.info("CRC failed, attempting to send again.")
                clear_interrupts(board_connection, registers,
                                 crc=True, ocp=False)
        else:  # Assume the CRC has passed
            logging.debug("Not checking CRC status, continuing.")
            break

    if curr_attempt > max_attempts:
        raise ValueError("The power supply CRC failed the maximum times: "
                         "({}).".format(max_attempts))
