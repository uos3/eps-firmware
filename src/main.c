/*
 *
 * @file main.c
 * @author Roberto Holmes (rh3u17@soton.ac.uk / robertoholmes@live.com)
 * @brief Main file for EPS firmware.
 *
 * @version 0.1
 * @date 2021-03-29
 *
 * @copyright UOS3
 */


#include "main.h"


int main(void)
{
    /* ---- INITIALISATION ---- */

    /* Do msp430 initialisation */
	init_msp430();

    /* Do drivers init */
    init_drivers();

    /* Do component init */
    init_components();

    /* Do application init */
    init_applications();

    /* Check for startup reason */
    init_startup();


    /* ---- MAIN LOOP ---- */
    while (1) {

        /* For each flag check whether it is active and should be dealt with */

        /* Check for UART interrupt */
        if(interrupt_flags & BIT0 != 0){
            Serial_event();
        }

        /* Check for watchdog timer interrupt */
        if(interrupt_flags & BIT1 != 0){
            TobcWatchdog_event();
        }

        /* Check for OCP interrupt */
        if(interrupt_flags & BIT2 != 0){
            Ocp_event();
        }

        /* Check for OBC pin interrupt */
        if(interrupt_flags & BIT3 != 0){
            TobcPin_event();
        }

        /* If everything is dealt with and the log has not been modified, go to sleep (in LPM3) with interrupts enabled */
        if(interrupt_flags == 0){
            if(modified_log)
            {
                LogFile_commit();
            }

            __bis_SR_register(LPM3_bits | GIE);
        }
    }
    return 0;
}
