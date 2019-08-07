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

void switches_init()        //What PSIN hold when direction is set to out
{
    PSDIR = 0xC0;      //Setting top two bits to output as they are unused and this prevents a floating input, reducing power consumption was oxc0 switched off state
    PSOUT = 0;
    PSIE = PSMASK;     //Whether flags are switched on for a pin
    PSIES = 0xFF;      //Setting flags to falling
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

void switches_set_direction(uint8_t mask)   //The direction i.e. whether the rails are set to be on or off by the mcu (no dependancy on overcurrent protection)
{
	PSDIR = map_to_port(mask);
}

uint8_t switches_get_flags()                //High to low transition on pin when direction is set to input (0) which means activated, most likely
{
	uint8_t flags = switchFlags;
	switchFlags &= ~flags;                  //Sets switchFlags to 0
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


//These swap the (first and third) and (4th and 6th), setting the 7th and 8th to 0 (counting 1st as the right most bit)
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
