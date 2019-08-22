/*
 * RailOffCheck.c
 *
 *  Created on: 15 Jul 2019
 *      Author: Giles Sohi gs6g17@soton.ac.uk
 */

//Based off of deploy.c for timing
#include "peripheral/ocp.h"
#include "peripheral/ocp.h"
#include <hal/stopwatchB.h>
#include <stdint.h>
#include "RailOffCheck.h"

static void restart_flight_mcu();

//uint8_t required_rails_on = 1;

//Indicates if it has been detected that the TOBC rail is switched off
uint8_t timer_started = 0;

const uint8_t reqRails = (1<<3);

//Interrupt located in switches

//This routine will be functional once MCU_int is rerouted to pin 1.6 (27) instead of 3.6
inline void TOBCWatchdogService() {
    //checks the TOBC line isn't already off
    if (!timer_started) {
        //Requires a kick every 60 secs switches off rail then checkTOBC off takes over switching on
        stopwatch_startB(TICKS_PER_SECOND * 60, &restart_flight_mcu);
    }
}

void checkTOBCOff() {
    //Switches the ToBC rail off when it is on and services the watchdog
    uint8_t railState = ocp_get_rails_activated();

/*    if ((railState & reqRails) != reqRails)   {
        required_rails_on = 0;
    }   else {
        required_rails_on = 1;
    }   */

    if ((!timer_started && ((railState & reqRails) ^ reqRails)))   {
        stopwatch_startB(TICKS_PER_SECOND * 5, &restart_flight_mcu);     //Stopwatch will trigger restart_flight _mcu() after 5s
        timer_started = 1;      //Shouldn't be needed unless the UART interrupt is being triggered while TOBC rail off
    }
}




static void restart_flight_mcu()    {
    ocp_toggle_rail(reqRails);
    timer_started = 0;
    TOBCWatchdogService();
}
