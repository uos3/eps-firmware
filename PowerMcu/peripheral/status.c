/*
 * status.c
 *
 *  Created on: 31 Oct 2015
 *      Author: Tom
 */

#include "status.h"
#include "interrupt.h"
#include "hal/intpin.h"
#include "stddef.h"


static uint8_t status;
void status_set_bits(uint8_t mask)
{
	status |= mask;
	intpin_set(0);
	//interrupt_set_status(status);
}

void status_clear_bits(uint8_t mask)
{
	status &= ~mask;
	intpin_set(0);
	//interrupt_set_status(status);

}

uint8_t status_get()
{
	return status;
}
