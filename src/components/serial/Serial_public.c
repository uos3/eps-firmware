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

uint8_t SERIAL_RX_PACKET[SERIAL_RX_PACKET_MAX_LENGTH];
//uint8_t expected_crc[SERIAL_RX_PACKET_MAX_LENGTH];
uint8_t SERIAL_RX_PACKET_LENGTH;

/* -------------------------------------------------------------------------
 * FUNCTIONS
 * ------------------------------------------------------------------------- */

/* Add header and append CRC and then pass packet to the driver */
uint8_t Serial_TX(uint8_t *p_packet_in, uint8_t response_type_in,
                  uint8_t frame_number_in, uint8_t packet_length_in) {

    p_packet_in[0] = frame_number_in;
    p_packet_in[SERIAL_COMMAND_ADDRESS] = response_type_in;
    crc_encode(p_packet_in, packet_length_in);

    /* Send data */
    return Uart_send_bytes(p_packet_in, packet_length_in + CRC_LENGTH);
}

/* Verify and read the data in the RX buffer */
uint8_t Serial_read_RX(uint8_t *p_frame_number_out, uint8_t *p_valid_packet_out,
                       uint8_t *p_length_out, uint8_t *p_data_out) {
    uint8_t i;
    /* Output frame number used by TOBC */
    *p_frame_number_out = SERIAL_RX_PACKET[0];

    /* Check if CRC is valid (0 if valid, 1 if invalid) */
    *p_valid_packet_out = crc_decode(
            SERIAL_RX_PACKET,
            SERIAL_RX_PACKET_LENGTH + SERIAL_HEADER_LENGTH + 2);

//    for (i = 0; i < SERIAL_RX_PACKET_LENGTH + SERIAL_HEADER_LENGTH; i++) {
//        expected_crc[i] = SERIAL_RX_PACKET[i];
//    }

//    crc_encode(expected_crc, SERIAL_RX_PACKET_LENGTH + SERIAL_HEADER_LENGTH);

    /* Output packet length */
    *p_length_out = SERIAL_RX_PACKET_LENGTH;

    for (i = 0; i < SERIAL_RX_PACKET_LENGTH + SERIAL_HEADER_LENGTH; i++) {
        p_data_out[i] = SERIAL_RX_PACKET[i];
    }

    return 0;
}

/* Deal with an RX event by putting the data in the packet buffer */
uint8_t Serial_process_RX() {
    Uart_recv_bytes(SERIAL_RX_PACKET, 2);
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
        /* TODO: deal with invalid command */
        SERIAL_RX_PACKET_LENGTH = 0;
    }

    /* Fill the SERIAL_RX_PACKET with the payload and CRC and get return value from the driver */
    return Uart_recv_bytes(&SERIAL_RX_PACKET[2],
                           SERIAL_RX_PACKET_LENGTH + CRC_LENGTH);
}
