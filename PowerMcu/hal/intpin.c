/*
 * intpin.c
 *
 *  Created on: 31 Oct 2015
 *      Author: Tom
 */

#include <msp430.h>
#include <stdint.h>

void intpin_init()
{
    P3DIR |= BIT6;      //Setting it to output
	P3OUT |= BIT6;      //Outputting 1

}

void intpin_set(uint8_t interruptActive)        //High to low triggers TOBC interrrupt
{
	if (interruptActive)
	{
		P3OUT &= ~BIT6;     //Output 0

	}
	else
	{
		P3OUT |= BIT6;      //Output 1
	}
}

