/*
 *
 * @file Serial_public.c
 * @author Roberto Holmes (rh3u17@soton.ac.uk / robertoholmes@live.com)
 * @brief Public Serial component serial file
 *
 * Provide first level of processing to UART packets including
 * dealing with CRC.
 *
 * Task ref: [EPS_SW_1.6.1]
 *
 * @version 0.1
 * @date 2021-04-08
 *
 * @copyright UOS3
 */

/* -------------------------------------------------------------------------
 * INCLUDES
 * ------------------------------------------------------------------------- */

#include "Serial_public.h"

/* -------------------------------------------------------------------------
 * GLOBALS
 * ------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------
 * FUNCTIONS
 * ------------------------------------------------------------------------- */

void Serial_encode(uint8_t *p_packet_in, uint8_t response_type_in,
                   uint8_t frame_number_in, uint8_t packet_length_in) {
    /* Prepare Header */
    p_packet_in[0] = frame_number_in;
    p_packet_in[1] = response_type_in;
    /* Add CRC */
    Crc_encode(p_packet_in, packet_length_in);
}

/*  */
uint8_t Serial_TX_nominal_header() {
    SERIAL_AWAITING_CONTINUE = 1;
    return Uart_send_bytes(SERIAL_TX_NOMINAL_PACKET, 2);
}

uint8_t Serial_TX_nominal_payload() {
    SERIAL_AWAITING_CONTINUE = 0;
    return Uart_send_bytes(SERIAL_TX_NOMINAL_PACKET + 2,
                           SERIAL_TX_NOMINAL_PACKET_LENGTH);
}

/* Add header and append CRC and then pass packet to the driver */
uint8_t Serial_TX_unsolicited_header() {
    SERIAL_AWAITING_CONTINUE = 1;
    return Uart_send_bytes(SERIAL_TX_UNSOLICITED_PACKET, 2);
}

uint8_t Serial_TX_unsolicited_payload() {
    SERIAL_AWAITING_CONTINUE = 0;
    return Uart_send_bytes(SERIAL_TX_UNSOLICITED_PACKET + 2,
                    SERIAL_TX_UNSOLICITED_PACKET_LENGTH);
}

/* Verify and read the data in the RX buffer */
uint8_t Serial_read_RX(uint8_t *p_frame_number_out, uint8_t *p_valid_packet_out,
                       uint8_t *p_length_out, uint8_t *p_data_out) {
    uint8_t i;
    /* Output frame number used by TOBC */
    *p_frame_number_out = SERIAL_RX_PACKET[0];

    /* Check if CRC is valid (0 if valid, 1 if invalid) */
    *p_valid_packet_out = Crc_decode(
            SERIAL_RX_PACKET,
            SERIAL_RX_PACKET_LENGTH + SERIAL_HEADER_LENGTH + 2);

    /* Output packet length */
    *p_length_out = SERIAL_RX_PACKET_LENGTH + SERIAL_HEADER_LENGTH;

    for (i = 0; i < SERIAL_RX_PACKET_LENGTH + SERIAL_HEADER_LENGTH; i++) {
        p_data_out[i] = SERIAL_RX_PACKET[i];
    }

    return 0;
}

/* Deal with an RX event by putting the data in the packet buffer */
uint8_t Serial_process_RX() {
    /* Get the header and ensure it is the right length */
    if (Uart_recv_bytes(SERIAL_RX_PACKET, 2) != 0) {
        SERIAL_TX_NOMINAL_PACKET_LENGTH = SERIAL_RESPONSE_INVALID_HEADER
                + SERIAL_HEADER_LENGTH;
        Serial_encode(SERIAL_TX_NOMINAL_PACKET, SERIAL_RESPONSE_INVALID_HEADER,
                      SERIAL_RX_PACKET[0], SERIAL_TX_NOMINAL_PACKET_LENGTH);
        return SERIAL_RX_ERROR;
    }
    /* Check if the TOBC is expecting a payload */
    if (SERIAL_AWAITING_CONTINUE != 0) {
        if (SERIAL_RX_PACKET[1] == SERIAL_COMMAND_CONTINUE) {
            return SERIAL_CONTINUE_RECEIVED;
        }
        else {
            return SERIAL_INVALID_CONTINUE_RECEIVED;
        }
    }
    /* Check how long the expected payload length is */
    switch (SERIAL_RX_PACKET[1]) {
    case SERIAL_COMMAND_HOUSE_KEEPING:
        SERIAL_RX_PACKET_LENGTH = SERIAL_PAYLOAD_SIZE_HOUSE_KEEPING;
        break;
    case SERIAL_COMMAND_UPDATE_CONFIG:
        SERIAL_RX_PACKET_LENGTH = SERIAL_PAYLOAD_SIZE_UPDATE_CONFIG;
        break;
    case SERIAL_COMMAND_SET_RAIL:
        SERIAL_RX_PACKET_LENGTH = SERIAL_PAYLOAD_SIZE_SET_RAIL;
        break;
    case SERIAL_COMMAND_BATTERY_COMM:
        SERIAL_RX_PACKET_LENGTH = SERIAL_PAYLOAD_SIZE_BATTERY_COMM;
        break;
    case SERIAL_COMMAND_RESET_RAIL:
        SERIAL_RX_PACKET_LENGTH = SERIAL_PAYLOAD_SIZE_RESET_RAIL;
        break;
    default:
        /* Invalid command */
        SERIAL_TX_NOMINAL_PACKET[SERIAL_HEADER_LENGTH] = SERIAL_RX_PACKET[1];
        SERIAL_TX_NOMINAL_PACKET_LENGTH = SERIAL_PAYLOAD_SIZE_UNRECOGNISED_COMMAND
                + SERIAL_HEADER_LENGTH;
        Serial_encode(SERIAL_TX_NOMINAL_PACKET, SERIAL_RESPONSE_UNRECOGNISED_COMMAND,
                      SERIAL_RX_PACKET[0], SERIAL_TX_NOMINAL_PACKET_LENGTH);

        SERIAL_RX_PACKET_LENGTH = 0;
        return SERIAL_RX_ERROR;
    }

    /* Fill the SERIAL_RX_PACKET with the payload and CRC.
     * If the receive function failed, tell the TOBC  */
    if (Uart_recv_bytes(&SERIAL_RX_PACKET[2],
                        SERIAL_RX_PACKET_LENGTH + CRC_LENGTH) != 0) {
        SERIAL_TX_NOMINAL_PACKET_LENGTH = SERIAL_PAYLOAD_SIZE_INVALID_LENGTH
                + SERIAL_HEADER_LENGTH;
        Serial_encode(SERIAL_TX_NOMINAL_PACKET, SERIAL_RESPONSE_INVALID_LENGTH,
                      SERIAL_RX_PACKET[0], SERIAL_TX_NOMINAL_PACKET_LENGTH);
        return SERIAL_RX_ERROR;
    }
    return SERIAL_HEADER_RECEIVED;
}
