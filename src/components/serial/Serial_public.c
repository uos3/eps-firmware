/*
 * Serial_public.c
 *
 *  Created on: 15 Mar 2021
 *      Author: Roberto
 */

#include "Serial_public.h"

int Serial_TX(uint8_t *p_packet_in, uint8_t packet_length_in) {
    return 0;
}

uint8_t* Serial_RX(uint8_t *p_length_out, uint8_t* p_valid_packet_out) {
    uint8_t packet_length, valid_packet;
    packet_length = 2;
    *p_length_out = packet_length;
    *p_valid_packet_out = valid_packet;
    uint8_t *p_packet;
    return p_packet;
}
