/*
 * status.h
 *
 *  Created on: 31 Oct 2015
 *      Author: Tom
 */

#ifndef STATUS_H_
#define STATUS_H_

typedef enum
{
	CRC  = 0x0100,
	OC_7 = 0x0080,
	OC_6 = 0x0040,
	OC_5 = 0x0020,
	OC_4 = 0x0010,
	OC_3 = 0x0008,
	OC_2 = 0x0004,
	OC_1 = 0x0002,
	OC_0 = 0x0001
} status_t;

void status_set_bits(status_t mask);
void status_clear_bits(status_t mask);
status_t status_get();

#endif /* STATUS_H_ */
