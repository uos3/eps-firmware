/*
 * analogue.c
 *
 * Created: 01/11/2015 17:08:10
 *  Author: Tom
 */
#include "../hal/adc.h"
#include "pc.h"

void analogue_convert()
{
	adc_start();
}

void analogue_process()
{
	uint16_t *result = adc_try_get_results();
	if (result)
	{
		pc_tx_data(result);
	}
}
