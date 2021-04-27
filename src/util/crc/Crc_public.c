/*
 *
 * @file Crc_public.c
 * @author Roberto Holmes (rh3u17@soton.ac.uk / robertoholmes@live.com)
 * @brief Public Cyclic Redundancy Check source file
 *
 * Implement CRC-16-CCITT in accordance to ECSS-E-ST-70-41C
 *
 * Task ref: [EPS_SW_1.6.1]
 *
 * @version 0.1
 * @date 2021-04-18
 *
 * @copyright UOS3
 */

#include "Crc_public.h"

/* Unoptimised CRC version
 * One step unoptimised CRC
 * Data - Byte to be encoded
 * Syndrome - Original CRC syndrome */
uint16_t Crc(uint8_t Data, uint16_t Syndrome) {
    uint8_t icrc; /* Loop index */
    for (icrc = 0; icrc < 8; icrc++) {
        if ((Data & 0x80) ^ ((Syndrome & 0x8000) >> 8)) {
            Syndrome = ((Syndrome << 1) ^ 0x1021) & 0xFFFF;
        }
        else {
            Syndrome = (Syndrome << 1) & 0xFFFF;
        }
        Data = Data << 1;
    }
    return (Syndrome);
}

/* Encoding procedure
 * NOTE: Assumption is that enough memory has been allocated for byte
 * stream B to allow for generation of the checksum value.
 * The two checksum octets are placed in the destination field
 * (as Nth and Nth + 1 octet of byte stream B).
 * The destination field is also known as the packet error
 * control field.
 * B - Buffer
 * octets - Size of the buffer*/
void Crc_encode(uint8_t *B, uint32_t octets) {
    uint32_t index; /* Loop index */
    uint32_t Chk; /* CRC syndrome */
    Chk = 0xFFFF; /* Reset syndrome to all ones */
    for (index = 0; index < octets; index++)
        Chk = Crc(B[index], Chk); /* Unoptimised CRC */
    B[octets + 1] = Chk & 0xff;
    B[octets] = (Chk >> 8) & 0xff;
}

/* Decoding function using unoptimised CRC version
 * B - Buffer to be checked
 * octets - Length of the buffer including the CRC */
uint8_t Crc_decode(uint8_t *B, uint32_t octets) {
    /* Decoding procedure */
    /* The error detection syndrome, S(x) is given by: */
    /* S(x)=(x^16 * C¤(x) + x^n * L(x)) modulo G(x) */
    /* If S(x) = 0 then no error is detected. */
    uint32_t index; /* Loop index */
    uint8_t result; /* Result of the decoding */
    uint16_t Chk; /* CRC syndrome */
    Chk = 0xFFFF; /* Reset syndrome to all ones */
    for (index = 0; index < octets; index++) {
        Chk = Crc(B[index], Chk); /* Unoptimised CRC */
    }
    if (Chk == 0)
        result = CRC_NO_ERROR_DETECTED;
    else
        result = CRC_ERROR_DETECTED;
    return result;
}

void Crc_generate_invalid(uint8_t *p_packet_in, uint32_t length_in) {
    /* Create a valid CRC */
    Crc_encode(p_packet_in, length_in);
    /* Scuff the CRC so that it fails */
    p_packet_in[length_in + 1] ^= 0xA;
}

