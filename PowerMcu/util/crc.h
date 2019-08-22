/*
 * crc.h
 *
 *  Created on: 11 Oct 2015
 *      Author: Tom
 */

#ifndef UTIL_CRC_H_
#define UTIL_CRC_H_

#include <stdint.h>

uint16_t crc_generate(uint8_t *message, int offset, int length);

#endif /* UTIL_CRC_H_ */
