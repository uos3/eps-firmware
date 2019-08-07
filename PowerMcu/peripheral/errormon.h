/*
 * errormon.h
 *
 *  Created on: 31 Oct 2015
 *      Author: Tom
 */

#ifndef CRCMON_H_
#define CRCMON_H_

#include <stdint.h>

typedef enum
{
	CrcInvalid,
	PacketTimeout
} ERROR_t;

void errormon_set(ERROR_t error, uint16_t value);
uint16_t errormon_get(ERROR_t error);
void errormon_increment(ERROR_t error);

#endif /* CRCMON_H_ */
