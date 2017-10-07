/*
 * interrupt.h
 *
 *  Created on: 31 Oct 2015
 *      Author: Tom
 */

#ifndef INTERRUPT_H_
#define INTERRUPT_H_

#include "status.h"

typedef enum
{
	MASK_CRC  = 0x0100,
	MASK_OC_7 = 0x0080,
	MASK_OC_6 = 0x0040,
	MASK_OC_5 = 0x0020,
	MASK_OC_4 = 0x0010,
	MASK_OC_3 = 0x0008,
	MASK_OC_2 = 0x0004,
	MASK_OC_1 = 0x0002,
	MASK_OC_0 = 0x0001
} config_t;

config_t interrupt_get_mask();
void interrupt_set_mask(config_t value);
void interrupt_set_status(status_t status);

#endif /* INTERRUPT_H_ */
