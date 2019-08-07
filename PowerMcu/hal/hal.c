/*
 * hal.c
 *
 *  Created on: 23 Oct 2015
 *      Author: Tom
 */

#include <hal/stopwatch.h>
#include <hal/stopwatchB.h>
#include "adc.h"
#include "switches.h"
#include "intpin.h"
#include "mux.h"

void hal_init()
{
	adc_init();
	switches_init();
	intpin_init();
	mux_init();
	stopwatch_init();
	stopwatch_initB();
}

