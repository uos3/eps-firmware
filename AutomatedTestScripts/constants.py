# constants.py
"""A set of constants for the UoS^3 power supply test package.

Attributes:
    TIMEOUT_SEC (float): The serial connection timeout.
    TEST_BAUD_RATE (int): The serial connection baud rate.
    PACKET_ID_TO_TYPE (dict): The different header bytes and their
        corresponding packet types for packets from the test board.
    PACKET_TYPE_TO_ID (dict): The different packet types and their
        corresponding header bytes.
    PACKET_LENGTHS (dict): The payload lengths of different packet
        types.
    VOLTAGE_MIN (float): The minimum test board supply voltage.
    VOLTAGE_MAX (float): The maximum test board supply voltage.
    LOG_FORMAT (str): The logging format, for help see the logging
        package documentation.
"""
# The power supply specification required values
MAX_TRIP_TIME = 100.0E-6  # seconds
MAX_TRIP_CURRENT = (55.0E-3, 220.0E-3, 550.0E-3, 220.0E-3, 660.0E-3, 55.0E-3,
                    1650.0E-3, 1650.0E-3)  # in Amps
MIN_TRIP_CURRENT = (45.0E-3, 180.0E-3, 450.0E-3, 180.0E-3, 540.0E-3, 45.0E-3,
                    1450.0E-3, 1450.0E-3)  # in Amps

# Logging format : print the time, the logging level and the logging packet
LOG_FORMAT = '[%(asctime)-15s] %(levelname)s: %(message)s'
