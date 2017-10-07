/*
 * main.c
 *
 * Created: 27/10/2015 16:54:05
 * Author : Tom
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "hal/hal.h"
#include "peripheral/wdt.h"
#include "peripheral/pc.h"
#include "peripheral/dut.h"
#include "peripheral/analogue.h"
#include "peripheral/ocp.h"

int main(void)
{
	wdt_disable();
	hal_init();
	sei();
	
	if (MCUSR)
	{
		// Reset reason can be found here.
		MCUSR = 0;
	}
	
    while (1)
    {
		pc_process();
		dut_process();
		analogue_process();
		ocp_process();
    }
}

