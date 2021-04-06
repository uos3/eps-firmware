/*
 * @ingroup timer
 *
 * @file Timer1_public.c
 * @author Daniel Murphy (dm4g16@soton.ac.uk)
 * @brief Public source file for timer0 driver.
 *
 * @version 0.1
 * @date 2021-04-04
 *
 * @copyright UOS3
 */
/* -------------------------------------------------------------------------
 * INCLUDES
 * ------------------------------------------------------------------------- */

/* Standard library includes */
#include <stdio.h>
#include <stdint.h>
#include <msp430.h>

/* -------------------------------------------------------------------------
 * GLOBALS
 * ------------------------------------------------------------------------- */
/*Variable to store the value to count to*/
static uint32_t timeleft = 0;
/*Stores the callback function to be executed when timer expires (interrupt)*/
static void (*p_Timer1_callback_function_stored)();


/* -------------------------------------------------------------------------
 * FUNCTIONS
 * ------------------------------------------------------------------------- */


void Timer1_disable(void) {
    /*Disables interrupt, timer and resets the stored callback function */
    TA1CCTL0 = 0;
    p_Timer1_callback_function_stored = 0;
    TA1CTL = 0;
    timeleft = 0;
}

void Timer1_expiry_process(void) {
    /*If time has ran out and there is a valid callback function calls this
     * then disables timer
     */
    if (timeleft == 0 && p_Timer1_callback_function_stored) {
        p_Timer1_callback_function_stored();
        Timer1_disable();
    }
}


void Timer1_start(uint32_t stopvalue_in, void (*p_Timer0_callback_function)()) {
    /*Disable interrupt*/
    TA1CCTL0 = 0;
    /*Disable timer */
    TA1CTL = 0;
    /*Reset timer*/
    TA1R = 0;
    /*Store pointer to callback function*/
    p_Timer1_callback_function_stored = p_Timer0_callback_function;
    /*Store value to count to*/
    timeleft = stopvalue_in;
    /*Max value allowed as 16 bit is 0xFFFF*/
    if (timeleft > 0xFFFF) {
        TA1CCR0 = 0xFFFF;
    }
    else if (timeleft > 0) {
        TA1CCR0 = timeleft & 0xFFFF;
    }
    if (timeleft == 0) {
        Timer1_expiry_process();
    }
    else {
        /*Enable interrupt*/
        TA1CCTL0 = CCIE;
        /*Set source as ACLK, divider as 1 and count up mode*/
        TA1CTL = TASSEL_1 + ID_0 + MC_1;
    }
}

/*Idea for interrupt needs separate interrupt handler */
/*
#pragma vector=TIMER1_A0_VECTOR
__interrupt void Timer1_A0 (void)
{
    timeleft should = TA1CCR0
    timeLeft -= TA1CCR0;

    if (timeLeft > 0xFFFF)
        TA1CCR0 = 0xFFFF;
    else if (timeLeft > 0)
        TA1CCR0 = timeLeft & 0xFFFF;
    else
    {
        TA1CTL = 0;
        TODO: interrupt should wake up and call Timer1_process
         core_check_wakeup(TIMER 1);
    }
}
*/
