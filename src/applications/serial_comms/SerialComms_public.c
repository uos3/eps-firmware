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

uint8_t SERIAL_COMMS_RX_PACKET[SERIAL_RX_PACKET_MAX_LENGTH];

void SerialComms_continue(uint8_t valid_continue_in) {
    /* If the continue byte has not been corrupted */
    if (valid_continue_in == SERIAL_COMMS_VALID_CONTINUE) {
        /* If the continue is for an unsolicited packet */
        if (SERIAL_RX_PACKET[0] == 0) {
            Serial_TX_unsolicited_payload();
        }
        /* Else it's for a nominal packet */
        else {
            Serial_TX_nominal_payload();
        }
    }
    else {
        if (SERIAL_RX_PACKET[0] == 0) {
            /* Create an invalid CRC so that the OBC knows that the
             * continue failed */
            Crc_generate_invalid(SERIAL_TX_UNSOLICITED_PACKET,
                                 SERIAL_TX_UNSOLICITED_PACKET_LENGTH);
            Serial_TX_unsolicited_payload();
        }
        else {
            Crc_generate_invalid(SERIAL_TX_NOMINAL_PACKET,
                                 SERIAL_TX_NOMINAL_PACKET_LENGTH);
            Serial_TX_nominal_payload();
        }
    }
}

void SerialComms_prepare_packet() {
    /* Length of RX and then TX packet */
    uint8_t length = 0;
    /* Store outcome of CRC check */
    uint8_t valid_packet = 0;
    volatile uint8_t response = 0;
    uint8_t frame_number = 0;

    /* Get received data */
    Serial_read_RX(&frame_number, &valid_packet, &length,
                   SERIAL_COMMS_RX_PACKET);

    /* Check if CRC was passed */
    if (valid_packet == CRC_ERROR_DETECTED) {
        /* If not, tell the TOBC */
        SERIAL_TX_NOMINAL_PACKET_LENGTH = SERIAL_PAYLOAD_SIZE_CORRUPTED_DATA
                + SERIAL_HEADER_LENGTH;
        response = SERIAL_RESPONSE_CORRUPTED_DATA;
    }
    else {

        /* Go through responses for each command that could be received */
        switch (SERIAL_COMMS_RX_PACKET[1]) {

        /* ----- TOBC commands to update the config file ----- */
        case SERIAL_COMMAND_UPDATE_CONFIG: {
            /* Write data from the packet to the config */
            ConfigFile_write (&SERIAL_COMMS_RX_PACKET[SERIAL_HEADER_LENGTH]);

            /* Read the OCP states from the config */
            ConfigFile_read_8bit(
                    CONFIG_FILE_RESET_RAIL_AFTER_OCP,
                    &SERIAL_TX_NOMINAL_PACKET[SERIAL_HEADER_LENGTH]);

            /* Read the TOBC timer from the config */
            uint16_t tobc_timer;
            ConfigFile_read_16bit(CONFIG_FILE_TOBC_TIMER, &tobc_timer);
            Convert_16bit_to_8bit(
                    tobc_timer, &SERIAL_TX_NOMINAL_PACKET[SERIAL_HEADER_LENGTH],
                    1);
            SERIAL_TX_NOMINAL_PACKET_LENGTH = length;
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
            SERIAL_TX_NOMINAL_PACKET[SERIAL_HEADER_LENGTH] =
                    RAILS_CURRENT_STATE;
            SERIAL_TX_NOMINAL_PACKET_LENGTH = SERIAL_HEADER_LENGTH + 1;
            break;
        }

            /* ----- TOBC commands turning a rail off then on ----- */
        case SERIAL_COMMAND_RESET_RAIL: {
            /* Turn given rails off then on */
            RailEditor_set_rails(SERIAL_COMMS_RX_PACKET[SERIAL_HEADER_LENGTH],
                                 0);
            RailEditor_set_rails(SERIAL_COMMS_RX_PACKET[SERIAL_HEADER_LENGTH],
                                 1);

            response = SERIAL_RESPONSE_SET_RAIL;

            /* Set values in TX packet to be what rails are currently on */
            SERIAL_TX_NOMINAL_PACKET[SERIAL_HEADER_LENGTH] =
                    RAILS_CURRENT_STATE;
            SERIAL_TX_NOMINAL_PACKET_LENGTH = SERIAL_HEADER_LENGTH + 1;
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
            SERIAL_TX_NOMINAL_PACKET[SERIAL_HEADER_LENGTH] =
                    (uint8_t) (battery_response >> 4);
            SERIAL_TX_NOMINAL_PACKET[SERIAL_HEADER_LENGTH + 1] =
                    (uint8_t) battery_response;
            SERIAL_TX_NOMINAL_PACKET_LENGTH = SERIAL_HEADER_LENGTH + 2;
            break;
        }

            /* ----- TOBC requests house keeping data ----- */
        case SERIAL_COMMAND_HOUSE_KEEPING: {
            /* Create header for TX */
            response = SERIAL_RESPONSE_HOUSE_KEEPING;

            /* Ask housekeeping to append all data after the header */
            HouseKeeping_get_data(
                    &SERIAL_TX_NOMINAL_PACKET[SERIAL_HEADER_LENGTH]);

            /* Length of packet has changed as HK data is a different size to the RX */
            SERIAL_TX_NOMINAL_PACKET_LENGTH = SERIAL_HEADER_LENGTH
                    + SERIAL_TX_HOUSEKEEPING_LENGTH;
            break;
        }

            /* ----- Unrecognised command ----- */
        default: {
            /* Set the packet to be the command */
            SERIAL_TX_NOMINAL_PACKET[SERIAL_HEADER_LENGTH] =
                    SERIAL_COMMS_RX_PACKET[1];
            SERIAL_TX_NOMINAL_PACKET_LENGTH =
            SERIAL_PAYLOAD_SIZE_UNRECOGNISED_COMMAND + SERIAL_HEADER_LENGTH;
            response = SERIAL_RESPONSE_UNRECOGNISED_COMMAND;
            break;
        }
        }
    }
    Serial_encode(SERIAL_TX_NOMINAL_PACKET, response, frame_number,
                  SERIAL_TX_NOMINAL_PACKET_LENGTH);
    /* Send the header of the compiled packet and await a confirmation */
    Serial_TX_nominal_header();
}

