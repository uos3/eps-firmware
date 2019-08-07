/*
 * mux.c
 *
 *  Created on: 6 Nov 2015
 *      Author: Tom
 */

#include <msp430.h>
#include <stdint.h>
#include "mux.h"

#define MUX_PINS 0x2F

void mux_init()
{
	P2DIR |= MUX_PINS;
	mux_select(0);
}

void mux_select(uint8_t channel)
{
	// Pinout is:
	// P2.0 - Mux A4
	// P2.1 - Mux A3
	// P2.2 - Mux A2
	// P2.3 - Mux A1
	// P2.5 - Mux A0
	// Pinout on pcb is backward wrt the ports.. reverse channel and add gap over P2.4.
	channel = ((channel & 0x01) << 5) | ((channel & 0x02) << 2) | ((channel & 0x04) << 0) | ((channel & 0x08) >> 2) | ((channel & 0x10) >> 4);
	P2OUT = (P2OUT & ~MUX_PINS) | channel;
}
