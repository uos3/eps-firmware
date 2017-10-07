/*
 * status.c
 *
 *  Created on: 31 Oct 2015
 *      Author: Tom
 */

#include "status.h"
#include "interrupt.h"

static status_t status;

void status_set_bits(status_t mask)
{
	status |= mask;
	interrupt_set_status(status);
}

void status_clear_bits(status_t mask)
{
	status &= ~mask;
	interrupt_set_status(status);
}

status_t status_get()
{
	return status;
}
