/*
 * interrupt.c
 *
 *  Created on: 31 Oct 2015
 *      Author: Tom
 */

#include "interrupt.h"
#include "hal/intpin.h"

static config_t mask;
static status_t status;

static void refresh_pin();

void interrupt_set_status(status_t value)
{
	status = value;
	refresh_pin();
}

config_t interrupt_get_mask()
{
	return mask;
}

void interrupt_set_mask(config_t value)
{
	mask = value;
	refresh_pin();
}

static void refresh_pin()
{
	intpin_set(status & ~mask);
}
