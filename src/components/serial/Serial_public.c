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

uint8_t SERIAL_RX_PACKET[SERIAL_RX_PACKET_TOTAL_LENGTH];

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
    return Uart_send_bytes(p_packet_in, packet_length_in+CRC_LENGTH);

}

/* Verify and read the data in the RX buffer */
uint8_t* Serial_read_RX(uint8_t *p_frame_number_out, uint8_t *p_valid_packet_out, uint8_t *p_length_out) {

    /* Output frame number used by TOBC */
    *p_frame_number_out = SERIAL_RX_PACKET[0];

    /* Check if CRC is valid (0 if valid, 1 if invalid) */
    *p_valid_packet_out = crc_decode(SERIAL_RX_PACKET, SERIAL_RX_PACKET_TOTAL_LENGTH);

    /* Output packet length */
    *p_length_out = SERIAL_RX_DATA_LENGTH;

    return SERIAL_RX_PACKET;
}


/* Deal with an RX event by putting the data in the packet buffer */
uint8_t Serial_process_RX() {
    /* Fill the SERIAL_RX_PACKET with data and get return value from the driver */
     return Uart_recv_bytes(SERIAL_RX_PACKET, SERIAL_RX_DATA_LENGTH);
}
