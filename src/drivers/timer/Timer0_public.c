/*
 * @ingroup timer
 *
 * @file Timer0_public.c
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
static uint32_t timeleft = 0;
static void (*p_Timer0_callback_function_stored)();


/* -------------------------------------------------------------------------
 * FUNCTIONS
 * ------------------------------------------------------------------------- */
void Timer0_disable(void) {
    /*Disables interrupt, timer and resets the stored callback function */
    TA0CCTL0 = 0;
    p_Timer0_callback_function_stored = 0;
    TA0CTL = 0;
    timeleft = 0;
}

void Timer0_expiry_process(void) {
    /*If time has ran out and there is a valid callback function calls this
     * then disables timer
     */
    if (timeleft == 0 && p_Timer0_callback_function_stored) {
        p_Timer0_callback_function_stored();
        Timer0_disable();
    }
}



void Timer0_start(uint32_t stopvalue, void (*p_Timer0_callback_function)()) {
    /*Disable interrupt*/
    TA0CCTL0 = 0;
    /*Disable timer */
    TA0CTL = 0;
    /*Reset timer*/
    TA0R = 0;
    /*Store pointer to callback function*/
    p_Timer0_callback_function_stored = p_Timer0_callback_function;
    timeleft = stopvalue;
    /*Max value allowed as 16 bit is 0xFFFF*/
    if (timeleft > 0xFFFF) {
        TA0CCR0 = 0xFFFF;
    }
    else if (timeleft > 0) {
        TA0CCR0 = timeleft & 0xFFFF;
    }
    if (timeleft == 0) {
        Timer0_expiry_process();
    }
    else {
        /*Enable interrupt*/
        TA0CCTL0 = CCIE;
        /*Set source as ACLK, divider as 1 and count up mode*/
        TA0CTL = TASSEL_1 + ID_0 + MC_1;
    }
}

/*Starting idea for interrupt TODO: define wakeup and create separate interrupt handlers*/
/*
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer0_A0 (void)
{
    timeleft should = TA0CCR0
    timeLeft -= TA0CCR0;

    if (timeLeft > 0xFFFF)
        TA0CCR0 = 0xFFFF;
    else if (timeLeft > 0)
        TA0CCR0 = timeLeft & 0xFFFF;
    else
    {
        TA0CTL = 0;
        TODO: interrupt should wake up and call Timer0_process
         core_check_wakeup(STOPWATCH);
    }
}
*/
