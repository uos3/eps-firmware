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

uint8_t required_rails_on = 1;
uint8_t timer_started = 0;

const uint8_t reqRails = (1<<3);


void checkTOBCOff() {
    uint8_t railState = ocp_get_rails_activated();

    if ((railState & reqRails) != reqRails)   {
        required_rails_on = 0;
    }   else {
        required_rails_on = 1;
    }

    if (!(timer_started || required_rails_on))   {
        stopwatch_startB(TICKS_PER_SECOND * 5, &restart_flight_mcu);     //Stopwatch will trigger restart_flight _mcu() after 5s
        timer_started = 1;      //Shouldnt be needed unless the UART interrrupt is being triggered while
    }
}



static void restart_flight_mcu()    {
    ocp_toggle_rail(reqRails);
    timer_started = 0;
}
