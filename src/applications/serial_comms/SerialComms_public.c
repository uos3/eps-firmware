/*
 *
 * @file SerialComms_public.c
 * @author Roberto Holmes (rh3u17@soton.ac.uk / robertoholmes@live.com)
 * @brief Deal with request from main to process serial data.
 *
 * @version 0.1
 * @date 2021-04-01
 *
 * @copyright UOS3
 */

#include "SerialComms_public.h"

int SerialComms_process() {
    /* Length of RX and then TX packet */
    uint8_t length = 0;
    /* 0 if packet failed CRC */
    uint8_t valid_packet = 0;
    uint8_t volatile response = 0;
    uint8_t frame_number = 0;

    /* Get received data */
    Serial_read_RX(&frame_number, &valid_packet, &length,
                   SERIAL_COMMS_RX_PACKET);

    /* Check if CRC was passed */
    if (valid_packet == CRC_ERROR_DETECTED) {
        /* If not, tell the TOBC */
        length = SERIAL_PAYLOAD_SIZE_CORRUPTED_DATA + SERIAL_HEADER_LENGTH;
        Serial_TX(SERIAL_COMMS_PACKET, SERIAL_RESPONSE_CORRUPTED_DATA, SERIAL_UNSOLICITED_FRAME_NUM, length);
        return 1;
    }

    /* Go through responses for each command that could be received */
    switch (SERIAL_COMMS_RX_PACKET[1]) {

    /* ----- TOBC commands to update the config file ----- */
    case SERIAL_COMMAND_UPDATE_CONFIG: {
        /* Write data from the packet to the config */
        ConfigFile_write(&SERIAL_COMMS_RX_PACKET[SERIAL_HEADER_LENGTH]);

        /* Read the OCP states from the config */
        ConfigFile_read_8bit(CONFIG_FILE_RESET_RAIL_AFTER_OCP,
                             &SERIAL_COMMS_PACKET[SERIAL_HEADER_LENGTH]);

        /* Read the TOBC timer from the config */
        uint16_t tobc_timer;
        ConfigFile_read_16bit(CONFIG_FILE_TOBC_TIMER, &tobc_timer);
        Convert_16bit_to_8bit(tobc_timer,
                              &SERIAL_COMMS_PACKET[SERIAL_HEADER_LENGTH],
                              1);

        /* Set the response data type to updated config */
        response = SERIAL_RESPONSE_UPDATE_CONFIG;
        break;
    }

        /* ----- TOBC commands setting rails ----- */
    case SERIAL_COMMAND_SET_RAIL: {
        /* Set required rail to given status */
        RailEditor_set_rails(0xFF,
                             SERIAL_COMMS_RX_PACKET[SERIAL_HEADER_LENGTH]);

        /* Set TX packet header */
        response = SERIAL_RESPONSE_SET_RAIL;

        /* Set values in TX packet to be what rails are currently on */
        SERIAL_COMMS_PACKET[SERIAL_HEADER_LENGTH] = RAILS_CURRENT_STATE;
        length = SERIAL_HEADER_LENGTH + 1;
        break;
    }

        /* ----- TOBC commands turning a rail off then on ----- */
    case SERIAL_COMMAND_RESET_RAIL: {
        /* Turn given rails off then on */
        RailEditor_set_rails(SERIAL_COMMS_RX_PACKET[SERIAL_HEADER_LENGTH], 0);
        RailEditor_set_rails(SERIAL_COMMS_RX_PACKET[SERIAL_HEADER_LENGTH], 1);

        response = SERIAL_RESPONSE_SET_RAIL;

        /* Set values in TX packet to be what rails are currently on */
        SERIAL_COMMS_PACKET[SERIAL_HEADER_LENGTH] = RAILS_CURRENT_STATE;
        length = SERIAL_HEADER_LENGTH + 1;
        break;
    }

        /* ----- TOBC requests communication with the battery ----- */
    case SERIAL_COMMAND_BATTERY_COMM: {
        response = SERIAL_RESPONSE_BATTERY_COMM;

        /* Create a 1 byte command and 2 byte battery input data value */
        uint8_t volatile battery_command =
                SERIAL_COMMS_RX_PACKET[SERIAL_HEADER_LENGTH];
        uint16_t volatile battery_input_data =
                ((uint16_t) SERIAL_COMMS_RX_PACKET[SERIAL_HEADER_LENGTH + 1])
                        << 8;
        battery_input_data |=
                (uint16_t) SERIAL_COMMS_RX_PACKET[SERIAL_HEADER_LENGTH + 2];

        /* Send command to battery and get response */
        uint16_t battery_response = BatteryComms_TX_RX(battery_command,
                                                       battery_input_data);

        /* Add battery response to TX packet */
        SERIAL_COMMS_PACKET[SERIAL_HEADER_LENGTH] = (uint8_t) (battery_response
                >> 4);
        SERIAL_COMMS_PACKET[SERIAL_HEADER_LENGTH + 1] =
                (uint8_t) battery_response;

        break;
    }

        /* ----- TOBC requests house keeping data ----- */
    case SERIAL_COMMAND_HOUSE_KEEPING: {
        /* Create header for TX */
        response = SERIAL_RESPONSE_HOUSE_KEEPING;

        /* Ask housekeeping to append all data after the header */
        HouseKeeping_get_data(&SERIAL_COMMS_PACKET[SERIAL_HEADER_LENGTH]);

        /* Length of packet has changed as HK data is a different size to the RX */
        length = SERIAL_HEADER_LENGTH + HOUSE_KEEPING_DATA_LENGTH;
        break;
    }

        /* ----- Unrecognised command ----- */
    default: {
        /* Set the packet to be the command */
        SERIAL_COMMS_PACKET[SERIAL_HEADER_LENGTH] = SERIAL_COMMS_RX_PACKET[1];
        length = SERIAL_PAYLOAD_SIZE_UNRECOGNISED_COMMAND + SERIAL_HEADER_LENGTH;
        Serial_TX(SERIAL_COMMS_PACKET, SERIAL_RESPONSE_UNRECOGNISED_COMMAND, SERIAL_UNSOLICITED_FRAME_NUM, length);
        return 2;
    }
    }
    __no_operation();
    /* Send compiled packet and return the status */
    return Serial_TX(SERIAL_COMMS_PACKET, response, frame_number, length);
}

