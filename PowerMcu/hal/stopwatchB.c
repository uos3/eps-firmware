/*
 * stopwatch.c
 *
 *  Created on: 8 Jan 2016
 *      Author: Tom
 */

#include <msp430.h>
#include <stdint.h>
#include "core.h"
#include "stopwatchB.h"
#include "peripheral/ocp.h"
static uint32_t timeLeft = 0;

static void (*stopwatch_callback_stored)();




void stopwatch_initB()
{
}

void stopwatch_startB(uint32_t stopValue, void (*stopwatch_callback)())
{
	TA1CCTL0 = 0; // Disable interrupt
	TA1CTL = 0;   // Disable timer.
	TA1R = 0;      // Reset timer.

	stopwatch_callback_stored = stopwatch_callback;
	timeLeft = stopValue;
	if (timeLeft > 0xFFFF)
		TA1CCR0 = 0xFFFF;
	else if (timeLeft > 0)
		TA1CCR0 = timeLeft & 0xFFFF;

	if (timeLeft == 0)
		stopwatch_processB();
	else
	{
		TA1CCTL0 = CCIE; // Enable interrupt.
		TA1CTL = TASSEL_1 // Source from ACLK./////////////////////////////////////////////////////////
				| ID_0    // Divider /1.
				| MC_1;   // Up mode: the stopwatch counts up to TACCR0
	}

}

void stopwatch_processB()
{
	if (timeLeft == 0 && stopwatch_callback_stored)
	{
		stopwatch_callback_stored();
		stopwatch_disableB();
	}
}

void stopwatch_disableB()
{
	TA1CCTL0 = 0; // Disable interrupt
	stopwatch_callback_stored = 0;
	TA1CTL = 0;
	timeLeft = 0;
}

#pragma vector=TIMER1_A0_VECTOR
__interrupt void Timer1_A0 (void)
{
	timeLeft -= TA1CCR0;

	if (timeLeft > 0xFFFF)
		TA1CCR0 = 0xFFFF;
	else if (timeLeft > 0)
		TA1CCR0 = timeLeft & 0xFFFF;
	else
	{

		core_check_wakeup(STOPWATCHB);
	}
}
