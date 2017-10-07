/*
 * wdt.c
 *
 * Created: 02/11/2015 18:38:04
 *  Author: Tom
 */

#include <avr/io.h>
#include <avr/interrupt.h>

// From atmega644p datasheet, 8.8.2
// http://www.atmel.com/images/atmel-8011-8-bit-avr-microcontroller-atmega164p-324p-644p_datasheet.pdf
void wdt_disable(void)
{
	cli();
	/* Clear WDRF in MCUSR */
	MCUSR &= ~(1<<WDRF);
	/* Write logical one to WDCE and WDE */
	/* Keep old prescaler setting to prevent unintentional
	time-out */
	WDTCSR |= (1<<WDCE) | (1<<WDE);
	/* Turn off WDT */
	WDTCSR = 0x00;
}