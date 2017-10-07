#!/usr/bin/env python3
# automated_testing.py
"""Automated testing of the UoS^3 power supply."""

# General packages
import time
import logging
import csv
import serial
# UoS^3 test modules
import serial_interface
import test_board
import power_supply
import constants as const


RAILS_WITH_OCP = 6
# TODO: get these actual values...
MIN_UVP_ENABLE_VOLTAGE = 7.5  # V
MAX_UVP_ENABLE_VOLTAGE = 7.3  # V
MIN_UVP_DISABLE_VOLTAGE = 8.0  # V
MAX_UVP_DISABLE_VOLTAGE = 8.2  # V


def test_uvp(board_connection, registers):
    supply_voltages = [[] for _ in range(0, 8)]
    battery_voltages = []
    passed = True

    # TODO: confirm actual DAC values...
    battery_voltage = 225  # 8.2 V
    test_board.set_value(board_connection, test_board.VOLTAGE_DAC_ADDRESS,
                         battery_voltage)

    # Enable all rails, but don't draw any current
    for rail in range(1, 9):
        test_board.set_value(board_connection, rail, 0)
        power_supply.change_rail_status(board_connection, registers,
                                        rail, True)

    # Decrease the supply voltage until UVP is enabled, also take measurements
    for battery_voltage in range(225, 205, -1):  # 8.2-6.8 V
        test_board.set_value(board_connection, test_board.VOLTAGE_DAC_ADDRESS,
                             battery_voltage)

        uvp_enabled = not power_supply.check_connection(board_connection)

        if uvp_enabled:
            break
        else:
            # Get the rail voltages so we can check rails are being regulated
            # across a range of battery voltages
            # TODO: store a voltage rather than a DAC value
            battery_voltages.append(battery_voltage)
            for rail in range(1, 9):
                measurements = _get_measurements(board_connection,
                                                 registers, rail)
                voltage, current, test_voltage, battery = measurements
                supply_voltages[rail-1].append(voltage)
    uvp_enable_voltage = battery_voltage

    # Increase the supply voltage until UVP is disabled again - there should
    # be some hysteresis i.e. the enable and disable voltages are different
    for battery_voltage in range(battery_voltage, 225):
        test_board.set_value(board_connection, test_board.VOLTAGE_DAC_ADDRESS,
                             battery_voltage)
        uvp_enabled = not power_supply.check_connection(board_connection)
        if not uvp_enabled:
            break

    uvp_disable_voltage = battery_voltage

    if (uvp_enable_voltage < MIN_UVP_ENABLE_VOLTAGE or
            uvp_enable_voltage > MAX_UVP_ENABLE_VOLTAGE):
        logging.error("UVP enable voltage out of range.")
        passed = False

    if (uvp_disable_voltage < MIN_UVP_DISABLE_VOLTAGE or
            uvp_disable_voltage > MAX_UVP_DISABLE_VOLTAGE):
        logging.error("UVP disable voltage out of range.")
        passed = False

    # Save the data to a csv file
    logging.info("Saving trip time data to a CSV file.")
    with open("ocp_trip_time.csv", "w", newline='') as csv_file:
        csv_writer = csv.writer(csv_file)
        header = ["Battery Voltage (V)", "Rail 1 (V)", "Rail 2 (V)",
                  "Rail 3 (V)", "Rail 4 (V)", "Rail 5 (V)",
                  "Rail 6 (V)", "Rail 7 (V)", "Rail 8 (V)"]
        csv_writer.writerow(header)
        # Convert the supply voltages from lists of results for each rail to lists of
        # results for each battery voltage - transpose the data
        supply_voltages = [list(x) for x in zip(*supply_voltages)]
        for i in range(0, len(supply_voltages)):
            csv_writer.writerow([battery_voltage] + supply_voltages[i])

    return passed


def test_ocp_trip_time(board_connection, registers, repeats=100):
    """Test the trip times on all OCP rails and compare with the spec.

    Args:
        board_connection:
        registers (list):
        repeats (int):

    Returns:
        Bool:
    """
    passed = True
    trip_times = [[] for _ in range(0, RAILS_WITH_OCP)]

    for rail in range(1, RAILS_WITH_OCP+1):
        for _ in range(0, repeats):
            trip_time = _get_trip_time(board_connection, registers, rail)
            trip_times[rail-1].append(trip_time)

        print("Rail {}: ".format(rail), end='')
        if None in trip_times[rail-1]:
            print("FAIL (Rail failed to trip).")
        elif _trip_time_in_spec(max(trip_times[rail-1])):
            print("PASS (Longest trip time: {} s).".format(
                max(trip_times[rail-1])))

    # Save the data in a csv file
    logging.info("Saving trip time data to a CSV file.")
    with open("ocp_trip_time.csv", "w", newline='') as csv_file:
        csv_writer = csv.writer(csv_file)
        header = ["Repeat", "Rail 1", "Rail 2", "Rail 3", "Rail 4", "Rail 5",
                  "Rail 6"]
        csv_writer.writerow(header)

        trip_times = [list(x) for x in zip(*trip_times)]
        for i in range(0, len(trip_times)):
            csv_writer.writerow([i] + trip_times[i])

    return passed


def _wait_for_settle(board_connection):
    # Wait for the minimum settle time to allow the OCP to trip.
    time_elapsed = 0  # seconds
    while (not board_connection.ocp_tripped and
           time_elapsed < power_supply.SETTLE_TIME):
        time.sleep(1.0E-5)
        time_elapsed += 1.0E-5


def _get_trip_time(board_connection, registers, rail):
    # Return the trip time for a rail.
    trip_time = None
    logging.info("Getting trip time on rail {}.".format(rail))

    try:
        power_supply.clear_interrupts(board_connection, registers)
        board_connection.ocp_tripped = False
        test_board.set_value(board_connection, rail-1, 0)
        # Enable the supply for this rail
        power_supply.change_rail_status(board_connection, registers, rail,
                                        enabled=True)
        test_board.set_value(board_connection, rail-1, 255)

        # Make sure we wait at least the minimum waiting time to allow the
        # OCP to trip
        _wait_for_settle(board_connection)

        trip_time = None
        if board_connection.ocp_tripped:
            # rails_tripped = power_supply.request_ocp_info(
            #    board_connection, registers)
            # supply_rail = power_supply.TEST_BOARD_TO_SUPPLY_RAIL[rail-1]
            if board_connection.ocp_timer is not None:
                # Calculate the trip time in seconds
                trip_time = board_connection.ocp_timer
                trip_time /= test_board.TEST_MCU_FREQ
                logging.debug("OCP trip time: {} s".format(trip_time))
                # Reset the OCP timer
                board_connection.ocp_timer = None

    except TimeoutError as timeout:
        logging.exception(timeout)

    # Reset the current sink
    test_board.set_value(board_connection, rail-1, 0)

    # Disable the given rail, all rails are now disabled
    power_supply.change_rail_status(board_connection, registers, rail,
                                    enabled=False)

    return trip_time


def sweep_current(board_connection, registers, rail, measure=False):
    """Test that the power supply is regulated over a current range.

    Range the current drawn from 0 A up to a maximum current linearly
    on a single line. Check to ensure the voltage is regulated within
    the specified limits.  If the OCP trips the test terminates.  For
    the test to pass, all measurements must be within the accuracy
    required by the specification, and the OCP must trip within both
    the minimum / maximum trip current and before the maximum trip
    time.  Note that this test assumes all other rails are disabled,
    and only considers one rail at a time.

    Args:
        board_connection (TestBoardConnection): An object containing
            the connection stream and its state.
        registers: The power supply MCU registers.
        rail (int): The rail (1-8) to sweep current on.
        supply_voltage (int): The supply voltage (DAC value) which
            the test board will use while it sweeps the current on one
            rail.

    Returns:
        Bool: True if the test has passed, False otherwise.
    """
    passed = True
    rails_tripped = [False for _ in range(0, RAILS_WITH_OCP)]
    current = 0.0  # A
    measured_currents = []
    measured_voltages = []
    battery_voltages = []

    try:
        logging.info("Sweeping current on rail {}.".format(rail))
        # Enable the supply for this rail
        power_supply.clear_interrupts(board_connection, registers)
        power_supply.change_rail_status(board_connection, registers, rail,
                                        enabled=True)
        board_connection.ocp_tripped = False

        for dac_value in range(0, 256):
            supply_currents = [0 for _ in range(0, 8)]
            supply_currents[rail-1] = dac_value
            current = test_board.set_value(board_connection, rail-1, dac_value)
            logging.info("Testing with draw of {} A.".format(current))
            # Make sure we wait at least the minimum waiting time to allow the
            # OCP to trip
            _wait_for_settle(board_connection)

            if board_connection.ocp_tripped:
                rails_tripped = power_supply.request_ocp_info(board_connection,
                                                              registers)
                # Stop testing once OCP is activated - no more current can be
                # drawn from this rail
                break
            elif measure:
                measurements = _get_measurements(board_connection,
                                                 registers, rail)
                (supply_voltage, supply_current,
                 test_voltages, battery_voltage) = measurements
                # Check the sanity of the measurements and that
                # the rails are being regulated at their correct
                # voltages
                _check_measurements(supply_voltage, supply_current,
                                    test_voltages, current, rail)
                _check_regulated_voltage(supply_voltage, rail)

                measured_voltages.append(supply_voltage)
                measured_currents.append(supply_current)
                battery_voltages.append(battery_voltage)

    except TimeoutError as timeout:
        logging.exception(timeout)
        logging.error("Test failed: Serial connection failed.")
        passed = False

    test_board.set_value(board_connection, rail-1, 0)
    # If OCP is active on this line check that it has been tripped within the
    # acceptable current range
    supply_rail = power_supply.TEST_BOARD_TO_SUPPLY_RAIL[rail-1]
    if supply_rail <= RAILS_WITH_OCP:  # if this rail has OCP on it
        if not _check_ocp_tripped(board_connection, rails_tripped, rail):
            current = None
            passed = False
        # else: has passed

    # Disable the given rail, all rails are now disabled
    power_supply.change_rail_status(board_connection, registers, rail,
                                    enabled=False)
    print("Rail {}: ".format(rail), end='')
    if passed:
        logging.info("Current sweep on rail {}: PASS.".format(rail))
        print("PASS.")
    else:
        logging.error("Current sweep on rail {}: FAIL".format(rail))
        print("FAIL.")

    # Write the current measurements to a CSV file
    if measure:
        logging.info("Saving voltage measurements to a CSV file.")
        with open("sweep_rail_{}.csv".format(rail), "w", newline='') as csv_file:
            csv_writer = csv.writer(csv_file)
            csv_writer.writerow(("Current (A)", "Rail Voltage (V)",
                                 "5V Regulated (V)", "3V3 Regulated (V)",
                                 "Battery Voltage (V)"))

            # Convert the measured voltages from lists of results
            # for the rail and regulators to lists of results for each current
            # value - transpose the data
            for i in range(0, len(measured_voltages)):
                csv_writer.writerow([measured_currents[i]] +
                                    measured_voltages[i] +
                                    [battery_voltages[i]])

    # Return the trip current
    return current


def _check_ocp_tripped(board_connection, rails_tripped, rail):
    # Check that the OCP has tripped.
    supply_rail = power_supply.TEST_BOARD_TO_SUPPLY_RAIL[rail-1]
    if not board_connection.ocp_tripped:
        logging.error("OCP check failed: Rail was not tripped.")
        tripped = False
    elif not rails_tripped[supply_rail-1]:
        logging.error("OCP check failed: OCP trip not recorded by supply MCU.")
        tripped = False
    else:
        tripped = True

    return tripped


def _trip_current_in_spec(trip_current, rail):
    return (trip_current > const.MIN_TRIP_CURRENT[rail-1] and
            trip_current < const.MAX_TRIP_CURRENT[rail-1])


def _trip_time_in_spec(trip_time):
    return trip_time < const.MAX_TRIP_TIME


def _get_measurements(board_connection, registers, rail):
    # Measure the voltage and current at the power supply
    supply_measurements = power_supply.request_rail_measurements(
        board_connection, registers, rail)

    if rail <= 6:
        (_, _, rail_voltage, rail_current) = supply_measurements
    else:
        rail_voltage, rail_current = supply_measurements

    supply_voltage = [rail_voltage]
    supply_current = rail_current
    # Get the 3.3 V and 5 V regulated voltages
    reg_measurements = power_supply.request_rail_measurements(
        board_connection, registers, 7)
    supply_voltage.append(reg_measurements[0])
    reg_measurements = power_supply.request_rail_measurements(
        board_connection, registers, 8)
    supply_voltage.append(reg_measurements[0])

    (battery_voltage, _) = power_supply.request_battery_measurements(
        board_connection, registers)

    # Measure the voltage at the test board, the current is already
    # known from the set DAC value
    test_voltages = test_board.request_measurements(board_connection)

    return supply_voltage, supply_current, test_voltages, battery_voltage


def _check_measurements(supply_voltage, supply_current, test_board_voltages,
                        test_board_current, rail):
    # Check voltage and current measurements are within spec.
    #
    # Both the power supply and test board are capable of measuring
    # voltages and currents.  Use this data to check that the power
    # supply conforms to its specification.  The measurements must
    # be within a certain accuracy (5%).
    logging.info("Power supply measured: {0} V, {1} A".format(
        supply_voltage, supply_current))
    logging.info("Test board measured: {0} V, {1} A".format(
        test_board_voltages[rail-1], test_board_current))

    # Do a sanity cross check on the voltage measurements
    passed = True
    max_error = 1.0  # V
    error = abs(supply_voltage[0] - test_board_voltages[rail-1])

    if error > max_error:
        passed = False
        logging.warning("Measurement check failed: Difference of {} V "
                        "between supply and test board.".format(error))
    else:
        logging.debug("Voltage measurement matches test board "
                      "(difference: {}".format(error))

    # Cross check the known current draw roughly against the current
    # measurements
    max_error = 0.2 * test_board.MAX_DRAWN_CURRENT[rail-1]
    error = abs(supply_current - test_board_current)

    if error > max_error:
        passed = False
        logging.warning("Measurement check failed: Difference of {} A "
                        "between supply and test board.".format(error))
    else:
        logging.debug("Current measurement matches test board "
                      "(difference: {} V)".format(error))

    return passed


def _check_regulated_voltage(supply_voltage, rail):
    """Check the rail voltage against the specification."""
    specified_voltage = test_board.RAIL_VOLTAGE[rail-1]
    error = (specified_voltage - supply_voltage[0]) / specified_voltage
    if abs(error) > 0.05:
        logging.error("Voltage check failed: "
                      "Supply voltage {} "
                      "differs by > 5%".format(supply_voltage))

        print("Supply voltage: {}".format(supply_voltage))
        print("Specified voltage: {}".format(specified_voltage))

        passed = False
    else:
        logging.debug("Voltage measurement in spec, error {}%".format(
                      error))
        passed = True

    return passed


def _run(print_log=False, verbose=False):
    # Run a set of tests
    verbose = True
    repeats = 1

    # Set up logging
    if verbose:
        logging_level = logging.DEBUG
    else:
        logging_level = logging.INFO
    logging.basicConfig(filename="automated_test.log", format=const.LOG_FORMAT,
                        filemode='w', level=logging_level)
    if print_log:
        # Output logging to std err (console)
        logging.getLogger().addHandler(logging.StreamHandler())

    # Open and check the test board connection
    logging.debug("Opening the serial connection on COM6.")
    board_connection = serial_interface.open_connection("COM6")
    print("*** Checking test board connection: ", end='')
    if test_board.check_connection(board_connection):
        print("OK. ***")
    else:
        print("FAILED. ***")
        # If the connection fails there's no point continuing
        # testing
        raise Exception("Test board connection failed.")
    print("Initialising supply values.")  # DEBUG
    test_board.initialise(board_connection)

    # Give the power supply enough time for the deployment timer
    # to activate the supply
    print("*** Waiting 10 seconds for power supply to activate. ***")
    time.sleep(10.0)

    # Initialise the power supply
    print("*** Checking the power supply MCU is active: ", end='')
    if power_supply.check_connection(board_connection):
        print("OK. ***")
    else:
        print("FAILED. ***")
        raise Exception("Power supply connection failed.")
    registers, battery = power_supply.initialise(board_connection)

    print("*** Sweeping current on all rails. ***")
    # Sweep once with measurements
    #for rail in range(1, 9):
    #    sweep_current(board_connection, registers,
    #                  rail, measure=True)

    # Sweep multiple times but measuring the trip time
    for rail in range(1, RAILS_WITH_OCP+1):
        trip_currents = []
        for i in range(0, repeats):
            current = sweep_current(board_connection, registers,
                                    rail, measure=False)
            trip_currents.append(current)
            print("Trip current: {} A".format(current))

        with open("ocp_trip_current_rail{}.csv".format(rail),
                  "w", newline='') as csv_file:
            csv_writer = csv.writer(csv_file)
            header = ["Repeat", "Trip Current (A)"]
            csv_writer.writerow(header)
            for r in range(0, repeats):
                csv_writer.writerow((r, trip_currents[r]))

    print("*** Testing over-current protection trip time. ***")
    test_ocp_trip_time(board_connection, registers, repeats=repeats)
    # Enable when UVP is fixed
    #print("*** Testing under-voltage protection. ***")
    #test_uvp(board_connection, registers)

    board_connection.close()
    logging.debug("Tests finished.")


if __name__ == "__main__":
    try:
        _run()
    except serial.SerialException as serial_exception:
        print("COM port for test board failed to open ({})".format(
            serial_exception))
    #test_dac()
