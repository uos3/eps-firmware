#include <msp430.h>
#include "core.h"
#include "hal/hal.h"
#include "hal/watchdog.h"
#include "hal/stopwatch.h"
#include "peripheral/comm.h"
#include "peripheral/ocp.h"
#include "peripheral/deploy.h"
#include "peripheral/register.h"

#define SW_ON_FLIGHT_MCU SW_ON_0

int main(void)
{
	watchdog_stop();
	core_init();
	hal_init();
    comm_init();
    register_init();

    deploy_wait_to_activate(10);
    register_set(SW_ON, SW_ON_FLIGHT_MCU);

    for (;;)
    {
    	switch (core_sleep(UART | SWITCH | STOPWATCH))
    	{
    	case UART:
    		comm_process();
    		break;
    	case SWITCH:
    		ocp_process();
    		break;
    	case STOPWATCH:
    		stopwatch_process();
    		break;
    	}
	}
}
