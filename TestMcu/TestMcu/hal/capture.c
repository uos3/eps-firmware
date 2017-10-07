/*
 * capture.c
 *
 * Created: 02/11/2015 18:01:18
 *  Author: Tom
 */

#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#define CAPTURE_NOISE_CANCEL_ENABLE (1<<ICNC1)
#define CAPTURE_EDGE_RISING (1<<ICES1)
#define CAPTURE_EDGE_FALLING (0<<ICES1)
#define CLK_0    ((0<<CS12)|(0<<CS11)|(0<<CS10))
#define CLK_1    ((0<<CS12)|(0<<CS11)|(1<<CS10))
#define CLK_8    ((0<<CS12)|(1<<CS11)|(0<<CS10))
#define CLK_64   ((0<<CS12)|(1<<CS11)|(1<<CS10))
#define CLK_256  ((1<<CS12)|(0<<CS11)|(0<<CS10))
#define CLK_1024 ((1<<CS12)|(0<<CS11)|(1<<CS10))
#define TCCR1B_BITS CAPTURE_EDGE_RISING

enum captureStates
{
	STATE_STOPPED=0,
	STATE_OCP_TRIPPED,
	STATE_TIMING_CAPTURED
};

volatile uint8_t captureState = STATE_STOPPED;
volatile uint16_t overflowCounter;
volatile uint16_t capturedValue;

void capture_init()
{
	TCCR1A = 0;
	TCCR1B = TCCR1B_BITS | CLK_0;
	captureState = STATE_STOPPED;
}

void capture_reset()
{
	captureState = STATE_STOPPED;
	overflowCounter = 0;
	// Reset and restart the timer
	TCNT1 = 0;
	TCCR1B = TCCR1B_BITS | CLK_1;
	TIMSK1 = (1<<ICIE1) | (1<<TOIE1);
}

uint32_t capture_read()
{
	if (captureState == STATE_OCP_TRIPPED)
	{
		uint32_t tripTime = ((uint32_t) overflowCounter << 16);

		tripTime = (uint32_t) (overflowCounter) << 16;
		tripTime |= (uint32_t) capturedValue;
		captureState = STATE_TIMING_CAPTURED;

		return tripTime;
	}
	else return 0;
}

ISR(TIMER1_CAPT_vect)
{
	if (captureState == STATE_STOPPED)
	{
		capturedValue = ICR1;
		captureState = STATE_OCP_TRIPPED;
		// Stop the timer
		TCCR1B = TCCR1B_BITS | CLK_0;
	}
}

ISR(TIMER1_OVF_vect)
{
	if (overflowCounter < 0xFFFF)
		overflowCounter++;
	else
	{
		// Stop the timer at its maximum value
		TCCR1B = TCCR1B_BITS | CLK_0;
		TCNT1 = 0xFFFF;
	}
}
