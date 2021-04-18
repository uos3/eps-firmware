/*
 *
 * @file Crc_public.h
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

#ifndef UTIL_CRC_CRC_PUBLIC_H_
#define UTIL_CRC_CRC_PUBLIC_H_

/* -------------------------------------------------------------------------
 * INCLUDES
 * ------------------------------------------------------------------------- */

/* Standard library includes */
#include <stdint.h>
#include <stdio.h>

/* -------------------------------------------------------------------------
 * Defines
 * ------------------------------------------------------------------------- */

#define CRC_ERROR_DETECTED 0
#define CRC_NO_ERROR_DETECTED 1

/* -------------------------------------------------------------------------
 * FUNCTIONS
 * ------------------------------------------------------------------------- */

uint16_t Crc(uint8_t Data, uint16_t Syndrome);
void crc_encode(uint8_t *B, uint32_t octets);
uint8_t crc_decode(uint8_t *B, uint32_t octets);

#endif /* UTIL_CRC_CRC_PUBLIC_H_ */
