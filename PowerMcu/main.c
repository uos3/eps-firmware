#include <msp430.h>
#include "core.h"
#include "hal/hal.h"
#include "hal/watchdog.h"
#include "hal/stopwatch.h"
#include "hal/stopwatchB.h"
#include "peripheral/comm.h"
#include "peripheral/ocp.h"
#include "peripheral/deploy.h"
#include "peripheral/register.h"
#include "peripheral/RailOffCheck.h"

//New
#include "hal/uart.h"
#include "hal/i2c.h"

#define SW_ON_TX            SW_ON_0
#define SW_ON_RX            SW_ON_1
#define SW_ON_CAM           SW_ON_2
#define SW_ON_FLIGHT_MCU    SW_ON_3
#define SW_ON_GPS           SW_ON_4
#define SW_ON_LNA           SW_ON_5

//For details for functionality see Internship 2019/software/EPS&TOBCSoftwareStatus
//TODO there are now 6 mppts which need voltage monitoring
//
int main(void)
{
	watchdog_stop();    //TODO try changing to watchdog service
	core_init();
	hal_init();
    comm_init();
    register_init();

//    register_set(SW_ON, SW_ON_FLIGHT_MCU);          //For normal running of TOBC
    register_set(SW_ON, 0b00111111);      //For testing working rails

//    register_set(SW_ON, SW_ON_BURADIO);
//    register_set(SW_ON, SW_ON_RADIO);
//    register_set(SW_ON, SW_ON_CAM);
//    register_set(SW_ON, SW_ON_FLIGHT_MCU);
//    register_set(SW_ON, SW_ON_GPS);
//    register_set(SW_ON, SW_ON_LNA);

    //uint16_t data = register_get(BAT_T);
    //data++;

    //TOBCWatchdogService();            //TODO, uncomment when interrupt line with TOBC set up, currently requires EPS routing changes and addition to TOBC code 12/08/2019
    //deploy_wait_to_activate(2);

    for (;;)    {

        //checkTOBCOff(); //TODO does this need to be added to core.c before it enters sleep
    	switch (core_sleep(UART | STOPWATCH | STOPWATCHB)) //| I2C_DEBUG))
    	{
    	case UART:
    		comm_process();
    		break;
    	case STOPWATCH:
    		stopwatch_process();
    		break;
    	case SWITCH:        //Called from switches.c
    	    ocp_process();
    	    break;
    	case STOPWATCHB:
    	    stopwatch_processB();
    	    break;
    	}
	}
}
