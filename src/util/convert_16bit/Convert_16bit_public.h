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

#ifndef UTIL_CONVERT_16BIT_CONVERT_16BIT_PUBLIC_H_
#define UTIL_CONVERT_16BIT_CONVERT_16BIT_PUBLIC_H_

/* -------------------------------------------------------------------------
 * INCLUDES
 * ------------------------------------------------------------------------- */

/* Standard Library */
#include <stdint.h>

/* -------------------------------------------------------------------------
 * FUNCTIONS
 * ------------------------------------------------------------------------- */

uint8_t Convert_16bit_to_8bit(uint16_t data_in, uint8_t *p_packet_out,
                              uint8_t array_index_inout);

#endif /* UTIL_CONVERT_16BIT_CONVERT_16BIT_PUBLIC_H_ */
