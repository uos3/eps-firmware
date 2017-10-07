#!/usr/bin/env python3 
# thermal_testing.py
"""Put programmable test board in state to load and stress DUT."""
import serial_interface
import power_supply
import test_board


def calculate_sink(current, rail):
    dac_value = current * 256.0 / test_board.MAX_DRAWN_CURRENT[rail-1]
    return dac_value


if __name__ == "__main__":
    connection = serial_interface.open_connection("COM6")
    registers, battery = power_supply.initialise(connection)

    test_board.set_value(connection, 0x8, test_board.DEFAULT_SUPPLY_VOLTAGE)

    typical = (20.0E-3, 120.0E-3, 300.0E-3, 100.0E-3, 350.0E-3, 20.0E-3, 100.0E-3, 1.0E-3)

    dac_values = [int(calculate_sink(typical[rail-1], rail)) for rail in range(1,9)]
    dac_values[4] = 255
    print(dac_values)

    # dac_values = (0, 0, 0, 0, 0, 0)
    for i in range(0, 6):
        power_supply.change_rail_status(connection, registers,
                                        i+1, enabled=True)
        test_board.set_value(connection, i, dac_values[i])


    connection.close()