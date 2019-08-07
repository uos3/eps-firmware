/*
 * interrupt.c
 *
 *  Created on: 31 Oct 2015
 *      Author: Tom
 */

#include "interrupt.h"
#include "hal/intpin.h"

uint8_t mask = 0x60;
uint8_t status;

static void refresh_pin();

void interrupt_set_status(uint8_t value)
{
	status = value;
	refresh_pin();
}

uint8_t interrupt_get_mask()
{
	return mask;
}

void interrupt_set_mask(uint8_t value)
{
	mask = value;
	refresh_pin();
}

//Status is 1
static void refresh_pin()
{
	intpin_set(status & ~mask);
}
