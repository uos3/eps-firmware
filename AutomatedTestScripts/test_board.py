# test_board.py
"""Functions to control the test board measurement / supply.

The test board controls the supply voltage and is able to measure
each rail under load.  This module provides useful test board
functions e.g. requesting measurements or calculating a current /
voltage value from a DAC or ADC value.
"""
# General packages
import time
import logging
# UoS test modules
import serial_interface


VOLTAGE_MIN = 6.0  # Volts
VOLTAGE_MAX = 8.2  # Volts
TEST_MCU_FREQ = 12.0E6  # Hz
VOLTAGE_DAC_ADDRESS = 0x8
RAIL_VOLTAGE = (3.3, 5.0, 3.3, 7.8, 3.3, 3.3, 5.0, 3.3)
MAX_DRAWN_CURRENT = (58.9E-3, 275.0E-3, 2.2, 589.3E-3, 1.65, 58.9E-3,
                     100.0E-3, 48.5E-3)  # in Amps
DEFAULT_SUPPLY_VOLTAGE = 215  # ~8.0 V


def check_connection(board_connection):
    """Check the test board is alive by sending a single packet.

    If the board is powered and working it should respond to the
    MEASURE packet sent.  This will not change the board
    output.

    Returns:
        Bool: True if board responded, False otherwise.
    """
    try:
        serial_interface.send_packet(board_connection, "MEASURE")
    except TimeoutError:
        passed = False
    else:
        passed = True

    return passed


def initialise(board_connection):
    logging.debug("Initialising test board.")
    set_value(board_connection, VOLTAGE_DAC_ADDRESS, DEFAULT_SUPPLY_VOLTAGE)
    # Set current draw for all rails to 0 A
    for curr_dac in range(0, VOLTAGE_DAC_ADDRESS):
        set_value(board_connection, curr_dac, 0)


def set_value(board_connection, address, new_value):
    """Set new values for a current sink or supply voltage.

    The test board is able to dynamically sink current on each load and
    regulate the supply at a programmed voltage.  Use a 'SET' packet to
    set these.

    Args:
        board_connection (TestBoardConnection): An object containing
            the connection stream and its state.
        address (int): The DAC address to set: 0-7 are current sinks, 8
            is the supply voltage.
        new_value (int): The DAC value.

    Returns:
        Float: The current / voltage that the load / supply has
            been set to.
    """
    logging.debug("Attempting to set test board supply value.")
    payload = [address, new_value]
    serial_interface.send_packet(board_connection, "SET", payload)

    logging.info("Test board rail {0} set to {1}".format(
        address, new_value))

    if address < VOLTAGE_DAC_ADDRESS:
        return _calculate_current_drawn(new_value, address+1)
    else:
        return _calculate_supply_voltage(new_value)


def request_measurements(board_connection):
    """Request voltage measurements from the test board.

    The test board is able to measure the voltage of each line.
    This measurement will be compared against the voltage measurements
    done by the power supply for consistency.

    Args:
        board_connection (TestBoardConnection): An object containing
            the connection stream and its state.
        registers (list): The last read power supply register values.

    Returns:
        List: A list of Floats containing the eight voltage
            measurements.
    """
    logging.debug("Requesting test board voltage measurements.")
    board_connection.clear_data_buffer()
    serial_interface.send_packet(board_connection, "MEASURE")

    # Wait for the test board to respond
    elapsed_time = 0.0
    while (not board_connection.data_received and
           elapsed_time < serial_interface.TIMEOUT_SEC):
        time.sleep(1.0E-3)
        elapsed_time += 1.0E-3

    # Fail if the data still hasn't been received
    if not board_connection.data_received:
        raise TimeoutError("Timed out waiting for test board voltages.")

    # Get the voltage measurements and clear the buffer
    adc_buffer = board_connection.data_buffer.copy()
    voltage_adc = [adc_buffer[i]
                   for i in range(0, len(adc_buffer), 2)]
    board_connection.clear_data_buffer()
    logging.debug("Test board voltage ADC values: {}".format(voltage_adc))

    voltages = []
    for rail in range(1, VOLTAGE_DAC_ADDRESS+1):
        voltages.append(_adc_to_voltage(voltage_adc[rail-1], rail))
    logging.info("Test board measured voltages: {}".format(voltages))

    return voltages


def _adc_to_voltage(adc_value, rail):
    # Calculate the measured voltage from an ADC reading.
    voltage = adc_value * (RAIL_VOLTAGE[rail-1] / 255.0)

    return voltage


def _calculate_current_drawn(dac_value, rail):
    # Calculate the current in Amps from the current sink DAC value.
    # The test board has eight programmable loads which are controlled
    # an output from a 12-address DAC.
    current = dac_value * (MAX_DRAWN_CURRENT[rail-1] / 255.0)

    return current


def _calculate_supply_voltage(dac_value):
    # Calculate the supply voltage in Volts from a DAC value.
    # A DAC is used to control the voltage regulator, setting the
    # voltage.  Convert this DAC value to a voltage.
    # TODO: Measure the voltage and get more accurate conversion -
    # the voltage minimum and maximum will probably be slightly
    # different to the expected values
    voltage_range = VOLTAGE_MAX - VOLTAGE_MIN
    voltage = VOLTAGE_MIN + dac_value * (voltage_range / 255.0)

    return voltage
