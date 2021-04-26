/*
 *
 * @file Convert_16bit_public.h
 * @author Roberto Holmes (rh3u17@soton.ac.uk / robertoholmes@live.com)
 * @brief Provide function to add 16 bit values to an 8 bit array. *
 *
 * @version 0.1
 * @date 2021-04-18
 *
 * @copyright UOS3
 */

#include "util/convert_16bit/Convert_16bit_public.h"

/* Add the ADC data the output packet in big endian */
uint8_t Convert_16bit_to_8bit(uint16_t data_in, uint8_t *p_packet_out,
                              uint8_t array_index_inout) {
    /* Add Most Significant Byte to the index and iterate it */
    p_packet_out[array_index_inout++] = (uint8_t) data_in;
    /* Add Least Significant Byte to the index and iterate it */
    p_packet_out[array_index_inout++] = (uint8_t) (data_in >> 8);
    /* Return the original index + 2 */
    return array_index_inout;
}

