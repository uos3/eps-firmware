/*
 *
 * @file Serial_public.c
 * @author Roberto Holmes (rh3u17@soton.ac.uk / robertoholmes@live.com)
 * @brief Provide first level of processing to UART packets including
 * dealing with CRC.
 *
 * @version 0.1
 * @date 2021-04-08
 *
 * @copyright UOS3
 */

#include "Serial_public.h"

/* Append CRC and then pass packet to the driver */
uint8_t Serial_TX(uint8_t *p_packet_in, uint8_t packet_length_in) {

    /* TODO: Add CRC */

    /* Send data */
    return Uart_send_bytes(p_packet_in, packet_length_in);

}

/*  */
uint8_t* Serial_RX(uint8_t *p_valid_packet_out, uint8_t *p_length_out) {

    /* Fill the SERIAL_PACKET with data and get return value from the driver */
    *p_valid_packet_out = Uart_recv_bytes(SERIAL_PACKET,
                                          SERIAL_PACKET_MAX_LENGTH);

    /* TODO: check CRC */

    /* Output packet length */
    *p_length_out = SERIAL_PACKET_MAX_LENGTH;

    return SERIAL_PACKET;
}
