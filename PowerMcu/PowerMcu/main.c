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

    register_set(SW_ON, SW_ON_GPS);

//    register_set(SW_ON, SW_ON_TX);
//    register_set(SW_ON, SW_ON_TX);
//    register_set(SW_ON, SW_ON_RX);
//    register_set(SW_ON, SW_ON_CAM);
//    register_set(SW_ON, SW_ON_FLIGHT_MCU);
//    register_set(SW_ON, SW_ON_GPS);
//    register_set(SW_ON, SW_ON_LNA);

    //uint8_t SLAVE_TO_COMM_WITH =


    uint8_t DATA = 69;
    uint8_t i = 0;

    // -- MASTER --
    i2c_masterInit(I2C_CLOCKSOURCE_SMCLK, 80, I2C_TRANSMIT_MODE);
//      UCB0I2CIE = UCNACKIE;
    i2c_disableRXInterrupt();
    i2c_enableTXInterrupt();

    uint16_t data = register_get(BAT_T);

    data++;

//    while (1) {
//        i2c_masterWrite(0x50, 9, &DATA);
//        uint8_t d[2] = {2,1};
//        //uart_tx(&d, 0, 2);
//        deploy_wait_to_activate(1);
//    }

    // -- SLAVE --
//    uint8_t* dataBuffer;
//    i2c_slaveInit(0x50, 9, dataBuffer);
//
//    while(1){
//        uint8_t temp = &dataBuffer;
//        temp++;
//        //for (i=0; i<1000; i++);
//    }

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

#pragma vector = USCIAB0RX_VECTOR
__interrupt void USCIAB0RX_ISR(void)
{
    // If UART (Port A) interrupt is enabled, and there is a pending interrupt
    if ((IFG2 & UCA0RXIFG) && (IE2 & UCA0RXIE)) {
        uart_handle_rx_interrupt();

        // Only available in ISR
        core_check_wakeup(UART);
    }

    // If interrupt was triggered for something else instead, most likely for I2C
    else {
        i2c_handle_rx_interrupt();
    }
}
