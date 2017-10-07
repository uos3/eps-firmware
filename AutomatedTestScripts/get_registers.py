import serial_interface
import power_supply

if __name__ == "__main__":
    connection = serial_interface.open_connection("COM6")
    registers = power_supply.initialise_registers(connection)

    for i in range(0, len(registers)):
        print("0x{0:x}: {1}".format(i, registers[i]))

    connection.close()