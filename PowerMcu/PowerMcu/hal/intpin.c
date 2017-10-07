/*
 * intpin.c
 *
 *  Created on: 31 Oct 2015
 *      Author: Tom
 */

#include <msp430.h>

void intpin_init()
{
	P3OUT |= BIT6;
	P3DIR |= BIT6;
}

void intpin_set(int interruptActive)
{
	if (interruptActive)
	{
		P3OUT &= ~BIT6;
	}
	else
	{
		P3OUT |= BIT6;
	}
}

