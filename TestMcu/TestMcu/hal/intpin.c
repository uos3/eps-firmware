/*
 * intpin.c
 *
 * Created: 28/10/2015 16:27:52
 *  Author: Tom
 */

#include <avr/io.h>
#include <avr/interrupt.h>

static uint8_t c = 0;

void intpin_init()
{
	DDRD &= ~_BV(PD4);
	PCMSK3 |= _BV(PCINT28);
	PCICR |= _BV(PCIE3);
}

uint8_t intpin_get()
{
	return !!(PIND & _BV(PD4));
}

uint8_t intpin_get_changed()
{
	return c;
}

void intpin_clear_changed()
{
	c = 0;
}

ISR(PCINT3_vect)
{
	c = 1;
}