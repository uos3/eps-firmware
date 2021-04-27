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

int main(void) {
    /* ---- INITIALISATION ---- */

    /* Do msp430 specific initialisation */
    init_msp430();

    /* Do drivers init */
    init_drivers();

    /* Do component init */
    init_components();

    /* Do application init */
    init_applications();

    /* Check for startup reason */
    init_startup();

    /* Enable interrupts */
    init_interrupts();

    /* ---- MAIN LOOP ---- */
    while (1) {

        /* Reset/kick timers */
        InterruptManager_reset_timer(INTERRUPT_MANAGER_WATCHDOG_TIMER | INTERRUPT_MANAGER_WAKE_TIMER);

        /* For each flag check whether it is active and should be dealt with */

        /* Check for UART interrupt */
        if ((INTERRUPTS_FLAGS & INTERRUPTS_UART_HEADER_FLAG) != 0) {
            INTERRUPTS_FLAGS &= ~INTERRUPTS_UART_HEADER_FLAG;  // Clear flag
            SerialEvent_event(SERIAL_EVENT_PREPARE_PACKET);
        }


        /* Check for UART interrupt */
        if ((INTERRUPTS_FLAGS & INTERRUPTS_UART_CONTINUE_FLAG) != 0) {
            INTERRUPTS_FLAGS &= ~INTERRUPTS_UART_CONTINUE_FLAG;  // Clear flag
            SerialEvent_event(SERIAL_EVENT_CONTINUE);
        }

        /* Check for UART interrupt */
        if ((INTERRUPTS_FLAGS & INTERRUPTS_UART_INVALID_CONTINUE_FLAG) != 0) {
            INTERRUPTS_FLAGS &= ~INTERRUPTS_UART_INVALID_CONTINUE_FLAG;  // Clear flag
            SerialEvent_event(SERIAL_EVENT_INVALID_CONTINUE);
        }

        /* Check for interrupt from timer acting as TOBC watchdog */
        if ((INTERRUPTS_FLAGS & INTERRUPTS_TOBC_TIMER_FLAG) != 0) {
            INTERRUPTS_FLAGS &= ~INTERRUPTS_TOBC_TIMER_FLAG;  // Clear flag
            TobcWatchdog_event();
        }

        /* Check for OCP interrupt */
        if ((INTERRUPTS_FLAGS & INTERRUPTS_OCP_FLAG) != 0) {
            INTERRUPTS_FLAGS &= ~INTERRUPTS_OCP_FLAG;  // Clear flag
            uint8_t ocp_status = (INTERRUPTS_FLAGS >> 4) & 0x3F; // Extract bits 5 through 7 from the flags
            Ocp_event(ocp_status);
        }

        /* Check for OBC pin interrupt */
        if ((INTERRUPTS_FLAGS & INTERRUPTS_TOBC_INT_FLAG) != 0) {
            INTERRUPTS_FLAGS &= ~INTERRUPTS_TOBC_INT_FLAG;  // Clear flag
            TobcPin_event();
        }

        /* Reset/kick timers */
        InterruptManager_reset_timer(INTERRUPT_MANAGER_WATCHDOG_TIMER | INTERRUPT_MANAGER_WAKE_TIMER);

        /* If everything is dealt with and the log has not been modified, go to sleep (in LPM3) with interrupts enabled */
        if ((INTERRUPTS_FLAGS & 0x0F) == 0) {
            if (LogFile_check_changes()) {
                LogFile_commit();
            }
            /* Go to sleep and enable listening to interrupts */
            __bis_SR_register(LPM3_bits | GIE);
        }
    }
}

