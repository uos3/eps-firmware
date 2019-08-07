/*
 * crc.c
 *
 *  Created on: 11 Oct 2015
 *      Author: Tom
 */
#include <stdint.h>
#include "crc.h"

uint16_t crc_generate(uint8_t *message, int offset, int length)
{
	uint16_t crcFull = 0xFFFF;
	int i, j;

	for (i = offset; i < length; i++)
	{
		crcFull = (uint16_t)(crcFull ^ message[i]);

		for (j = 0; j < 8; j++)
		{
			uint8_t crcLsb = (uint8_t)(crcFull & 0x0001);
			crcFull = (uint16_t)((crcFull >> 1) & 0x7FFF);

			if (crcLsb == 1)
				crcFull = (uint16_t)(crcFull ^ 0xA001);
		}
	}
	return crcFull;
}
