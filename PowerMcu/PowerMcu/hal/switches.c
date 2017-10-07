/*
 * switches.c
 *
 *  Created on: 11 Oct 2015
 *      Author: Tom
 */

//#include <msp430.h>
#include <stdint.h>
#include "switches.h"
#include "msp430f2132.h"
#include "core.h"

#define PSIN  P1IN
#define PSOUT P1OUT
#define PSDIR P1DIR
#define PSIE P1IE
#define PSIES P1IES
#define PSIFG P1IFG
#define PSMASK 0x3F

#define INPUT 0
#define OUTPUT 1

inline uint8_t map_to_port(uint8_t straight);
inline uint8_t map_from_port(uint8_t straight);

static volatile uint8_t switchFlags;

void switches_init()
{
	PSDIR &= ~PSMASK;
	PSOUT &= ~PSMASK;
	PSIE |= PSMASK;
	PSIES |= PSMASK;

	switchFlags = 0;
}

uint8_t switches_get()
{
	return map_from_port(PSIN);
}

uint8_t switches_get_direction()
{
	return map_from_port(PSDIR);
}

void switches_set_direction(uint8_t mask)
{
	PSDIR = map_to_port(mask);
}

uint8_t switches_get_flags()
{
	uint8_t flags = switchFlags;
	switchFlags &= ~flags;
	return map_from_port(flags);
}

#pragma vector=PORT1_VECTOR
__interrupt void PORT1_ISR(void)
{
	switchFlags |= PSIFG;
	// Clear flag
	PSIFG &= ~switchFlags;

	core_check_wakeup(SWITCH);
}

// Remapping the pins is required because the OCP
// numbers don't match up with the pin numbers.
// P1.0 = OCP3
// P1.1 = OCP2
// P1.2 = OCP1
// P1.3 = OCP6
// P1.4 = OCP5
// P1.5 = OCP4
inline uint8_t map_to_port(uint8_t straight)
{
	return (((straight & 0x01) << 2) |
	        ((straight & 0x02)     ) |
	        ((straight & 0x04) >> 2) |
	        ((straight & 0x08) << 2) |
	        ((straight & 0x10)     ) |
	        ((straight & 0x20) >> 2)) & PSMASK;
}
inline uint8_t map_from_port(uint8_t straight)
{
	return (((straight & 0x01) << 2) |
	        ((straight & 0x02)     ) |
	        ((straight & 0x04) >> 2) |
	        ((straight & 0x08) << 2) |
	        ((straight & 0x10)     ) |
	        ((straight & 0x20) >> 2)) & PSMASK;
}
