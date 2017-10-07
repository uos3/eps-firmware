/*
 * errormon.c
 *
 *  Created on: 31 Oct 2015
 *      Author: Tom
 */

#include <stdint.h>
#include "status.h"
#include "errormon.h"

#define ERROR_LAST PacketTimeout

static uint16_t errors[2];

void errormon_set(ERROR_t error, uint16_t value)
{
	if (error > ERROR_LAST)
		return;

	errors[error] = value;

	if (error == CrcInvalid && errors[CrcInvalid] != 0)
	{
		status_set_bits(CRC);
	}
}

uint16_t errormon_get(ERROR_t error)
{
	if (error > ERROR_LAST)
		return 0xFFFF;

	return errors[error];
}

void errormon_increment(ERROR_t error)
{
	if (error > ERROR_LAST)
		return;

	errormon_set(error, errormon_get(error) + 1);
}

