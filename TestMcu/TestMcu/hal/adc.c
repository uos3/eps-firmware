/*
 * adc.c
 *
 * Created: 01/11/2015 16:47:19
 *  Author: Tom
 */

#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#define ADMUXREF_2_56 (1<<REFS1) | (1<<REFS0)
#define ADPS_2 0
#define ADPS_128 (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0)
#define adc_convert(channel) do { \
	ADMUX = ADMUXREF_2_56 | (channel); \
	ADCSRA |= (1<<ADSC); \
	} while(0)

static uint8_t adcState = 0;
static uint16_t results[8];
static uint8_t resultIndex;

void adc_init()
{
	ADMUX = ADMUXREF_2_56;
	ADCSRA = (1<<ADEN) | (1<<ADIE) | ADPS_128;
	DIDR0 = 0xFF; // Disable all digital input buffers.
}

void adc_start()
{
	if (adcState != 1)
	{
		adcState = 1;
		resultIndex = 0;
		adc_convert(resultIndex);
	}
}

uint16_t *adc_try_get_results()
{
	if (adcState == 2)
	{
		adcState = 0;
		return results;
	}
	else
	{
		return 0;
	}
}

ISR(ADC_vect)
{
	results[resultIndex++] = ADC;
	
	if (resultIndex == 8)
	{
		adcState = 2;
	}
	else
	{
		adc_convert(resultIndex);
	}
}