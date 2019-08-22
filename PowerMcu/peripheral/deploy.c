/*
 * deploy.c
 *
 *  Created on: 8 Jan 2016
 *      Author: Tom
 */

#include <hal/stopwatch.h>
#include <stdint.h>
#include "core.h"

static void signal_stopped();
static uint8_t finished = 0;

void deploy_wait_to_activate(uint16_t seconds)
{
	stopwatch_start(TICKS_PER_SECOND * 10, &signal_stopped);
	for (;;)
	{
		switch (core_sleep(STOPWATCH))
		{
		case STOPWATCH:
			stopwatch_process();
			break;
		}
		if (finished)
			break;
	}
}

static void signal_stopped()
{
	finished = 1;
}
