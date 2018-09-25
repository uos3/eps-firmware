#include <msp430.h>
#include "core.h"
#include "hal/hal.h"
#include "hal/watchdog.h"
#include "hal/stopwatch.h"
#include "peripheral/comm.h"
#include "peripheral/ocp.h"
#include "peripheral/deploy.h"
#include "peripheral/register.h"

//New
#include "hal/uart.h"
#include "hal/i2c.h"

#define SW_ON_TX            SW_ON_0
#define SW_ON_RX            SW_ON_1
#define SW_ON_CAM           SW_ON_2
#define SW_ON_FLIGHT_MCU    SW_ON_3
#define SW_ON_GPS           SW_ON_4
#define SW_ON_LNA           SW_ON_5

int main(void)
{
	watchdog_stop();
	core_init();
	hal_init();
  comm_init();
  register_init();

  register_set(SW_ON, SW_ON_FLIGHT_MCU);

//    register_set(SW_ON, SW_ON_TX);
//    register_set(SW_ON, SW_ON_TX);
//    register_set(SW_ON, SW_ON_RX);
//    register_set(SW_ON, SW_ON_CAM);
//    register_set(SW_ON, SW_ON_FLIGHT_MCU);
//    register_set(SW_ON, SW_ON_GPS);
//    register_set(SW_ON, SW_ON_LNA);

    uint16_t data = register_get(BAT_T);

    data++;

    //deploy_wait_to_activate(10);

    for (;;)
    {
    	switch (core_sleep(UART | SWITCH | STOPWATCH | I2C_DEBUG))
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
//    	case I2C_DEBUG:
//
//    	    break;
    	}
	}
}
