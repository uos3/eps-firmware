/*
 *
 * @file InterruptManager_public.c
 * @author Roberto Holmes (rh3u17@soton.ac.uk / robertoholmes@live.com)
 * @brief Provide a level of abstraction for the interrupts.
 *
 * @version 0.1
 * @date 2021-04-05
 *
 * @copyright UOS3
 */

#include "InterruptManager_public.h"

void InterruptManager_reset_timer(uint8_t timer) {
    if (timer & INTERRUPT_MANAGER_WATCHDOG_TIMER != 0) {
        /* Resets (or kicks) the watchdog by passing the control
         * register the password (WDTPW) and the counter clear
         * command (WDTCNTCL)
         */
        WDTCTL = WDTPW + WDTCNTCL;
    }
    if (timer & INTERRUPT_MANAGER_WAKE_TIMER != 0) {
        /* Resets (or kicks) the timer by setting its counter to 0 */
        TA0R = 0;
    }
    if (timer & INTERRUPT_MANAGER_TOBC_TIMER != 0) {
        /* Resets (or kicks) the timer by setting its counter to 0 */
        TA1R = 0;
    }
}

