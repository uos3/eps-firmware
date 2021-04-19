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
    uint8_t response = 0;
    uint8_t frame_number = 0;
    /* Pointer to the packet that has been received */
    uint8_t *p_packet_rx;

    /* Get received data */
    p_packet_rx = Serial_read_RX(&frame_number, &valid_packet, &length);

    /* Check if data was received */
    if (length == 0) {
//        /* Set type of response in header */
//        SerialComms_add_header(SERIAL_COMMS_PACKET,
//        SERIAL_RESPONSE_NO_DATA);
//        /* Send packet which is just a header */
//        Serial_TX(SERIAL_COMMS_PACKET, SERIAL_HEADER_LENGTH);
        return 1;
    }

    /* Check if CRC was passed TODO: add to ICD*/
    if (valid_packet == 0) {
//        /* Set type of response in header */
//        SerialComms_add_header(SERIAL_COMMS_PACKET,
//        SERIAL_RESPONSE_CORRUPTED_DATA);
//        /* Send packet which is just a header */
//        Serial_TX(SERIAL_COMMS_PACKET, SERIAL_HEADER_LENGTH);
        return 2;
    }

    /* Go through responses for each command that could be received */
    switch (p_packet_rx[SERIAL_COMMAND_ADDRESS]) {

    /* ----- TOBC commands to update the config file ----- */
    case SERIAL_COMMAND_UPDATE_CONFIG: {
        /* Write data from the packet to the config */
        ConfigFile_write(&p_packet_rx[SERIAL_HEADER_LENGTH]);

        response = SERIAL_RESPONSE_UPDATE_CONFIG;

        /* Add written data to the data portion of the TX packet */
        uint8_t i;
        for (i = SERIAL_HEADER_LENGTH; i < length; i++) {
            SERIAL_COMMS_PACKET[i] = p_packet_rx[i];
        }
        break;
    }

        /* ----- TOBC commands setting rails ----- */
    case SERIAL_COMMAND_SET_RAIL: {
        /* Set required rail to given status */
        RailEditor_set_rails(p_packet_rx[SERIAL_HEADER_LENGTH],
                             p_packet_rx[SERIAL_HEADER_LENGTH + 1]);

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
        RailEditor_set_rails(p_packet_rx[SERIAL_HEADER_LENGTH], 0);
        RailEditor_set_rails(p_packet_rx[SERIAL_HEADER_LENGTH], 1);

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
        uint8_t battery_command = p_packet_rx[SERIAL_HEADER_LENGTH];
        uint16_t battery_input_data =
                ((uint16_t) p_packet_rx[SERIAL_HEADER_LENGTH + 1]) << 8;
        battery_input_data |= (uint16_t) p_packet_rx[SERIAL_HEADER_LENGTH + 2];

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
        /* TODO: add to ICD */
//        SerialComms_add_header(SERIAL_COMMS_PACKET,
//        SERIAL_RESPONSE_UNRECOGNISED_COMMAND);
//        Serial_TX(SERIAL_COMMS_PACKET, SERIAL_HEADER_LENGTH);
        return 3;
    }
    }
    /* Send compiled packet and return the status */
    return Serial_TX(SERIAL_COMMS_PACKET, response, frame_number, length);
}

