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

    P1DIR &= ~(TOBC_PIN | OCP1_PIN | OCP2_PIN | OCP3_PIN | OCP4_PIN | OCP5_PIN
            | OCP6_PIN);	 // Configure interrupts as input
    P1REN |= TOBC_PIN | OCP1_PIN | OCP2_PIN | OCP3_PIN | OCP4_PIN | OCP5_PIN
            | OCP6_PIN;  // Pull down if P1OUT.X = 0
    P1IE |= TOBC_PIN | OCP1_PIN | OCP2_PIN | OCP3_PIN | OCP4_PIN | OCP5_PIN
            | OCP6_PIN;	 // Enable pin interrupts
    P1IES |= TOBC_PIN | OCP1_PIN | OCP2_PIN | OCP3_PIN | OCP4_PIN | OCP5_PIN
            | OCP6_PIN;  // Interrupt on falling edge if 1
    P1IFG &= ~(TOBC_PIN | OCP1_PIN | OCP2_PIN | OCP3_PIN | OCP4_PIN | OCP5_PIN
            | OCP6_PIN);	 // Clear flags

    /* ---- MAIN LOOP ---- */
    while (1) {

        /* Reset/kick timers */
        InterruptManager_reset_timer(INTERRUPT_MANAGER_WATCHDOG_TIMER | INTERRUPT_MANAGER_WAKE_TIMER);

        /* For each flag check whether it is active and should be dealt with */

        /* Check for UART interrupt */
        if (INTERRUPT_FLAGS & UART_FLAG != 0) {
            INTERRUPT_FLAGS &= ~UART_FLAG;  // Clear flag
            SerialEvent_event();
        }

        /* Check for watchdog timer interrupt */
        if (INTERRUPT_FLAGS & WATCHDOG_FLAG != 0) {
            INTERRUPT_FLAGS &= ~WATCHDOG_FLAG;  // Clear flag
            TobcWatchdog_event();
        }

        /* Check for OCP interrupt */
        if (INTERRUPT_FLAGS & OCP_FLAG != 0) {
            INTERRUPT_FLAGS &= ~OCP_FLAG;  // Clear flag
            uint8_t ocp_status = (INTERRUPT_FLAGS >> 4) & 0x3F; // Extract bits 5 through 7 from the flags
            Ocp_event(ocp_status);
        }

        /* Check for OBC pin interrupt */
        if (INTERRUPT_FLAGS & TOBC_PIN_FLAG != 0) {
            INTERRUPT_FLAGS &= ~TOBC_PIN_FLAG;  // Clear flag
            TobcPin_event();
        }

        /* Reset/kick timers */
        InterruptManager_reset_timer(INTERRUPT_MANAGER_WATCHDOG_TIMER | INTERRUPT_MANAGER_WAKE_TIMER);

        /* If everything is dealt with and the log has not been modified, go to sleep (in LPM3) with interrupts enabled */
        if (INTERRUPT_FLAGS & 0x0F == 0) {
            if (LogFile_check_changes()) {
                LogFile_commit();
            }
            /* Go to sleep and enable listening to interrupts */
            __bis_SR_register(LPM3_bits | GIE);
        }
    }
}

/* -------------------------------------------------------------------------
 * Interrupts
 * ------------------------------------------------------------------------- */

/* Interrupt service routine to periodically wake up the EPS */
#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer_A_CCR0_ISR(void) {
    __bic_SR_register_on_exit(LPM3_bits); // Wake up (Need to test whether this is needed or if the registered get cleared anyway)
}

/* Interrupt service routine to reset the TOBC */
#pragma vector = TIMER1_A0_VECTOR
__interrupt void Timer_A_CCR1_ISR(void) {
    INTERRUPT_FLAGS |= WATCHDOG_FLAG;  // Set custom interrupt flag
    __bic_SR_register_on_exit(LPM3_bits); // Wake up
}

/* UART RX interrupt service routine */
#pragma vector = USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void) {
    INTERRUPT_FLAGS |= UART_FLAG; // Set custom interrupt flag for OCP event and specific rail
    __bic_SR_register_on_exit(LPM3_bits);  // Wake up
}

/* Port 1 interrupt service routine */
#pragma vector = PORT1_VECTOR
__interrupt void Port_1(void) {

    /* OCP Interrupts */
    if (P1IFG & OCP1_PIN != 0) {
        INTERRUPT_FLAGS |= OCP_FLAG | OCP1_FLAG; // Set custom interrupt flag for OCP event and specific rail
        P1IFG &= ~OCP1_PIN;	 // Clear default interrupt flag
    }
    if (P1IFG & OCP2_PIN != 0) {
        INTERRUPT_FLAGS |= OCP_FLAG | OCP2_FLAG;
        P1IFG &= ~OCP2_PIN;
    }
    if (P1IFG & OCP3_PIN != 0) {
        INTERRUPT_FLAGS |= OCP_FLAG | OCP3_FLAG;
        P1IFG &= ~OCP3_PIN;
    }
    if (P1IFG & OCP4_PIN != 0) {
        INTERRUPT_FLAGS |= OCP_FLAG | OCP4_FLAG;
        P1IFG &= ~OCP4_PIN;
    }
    if (P1IFG & OCP5_PIN != 0) {
        INTERRUPT_FLAGS |= OCP_FLAG | OCP5_FLAG;
        P1IFG &= ~OCP5_PIN;
    }
    if (P1IFG & OCP6_PIN != 0) {
        INTERRUPT_FLAGS |= OCP_FLAG | OCP6_FLAG;
        P1IFG &= ~OCP6_PIN;
    }

    /* TOBC Pin interrupt */
    if (P1IFG & TOBC_PIN != 0) {
        INTERRUPT_FLAGS |= TOBC_PIN_FLAG;  // Set custom interrupt flag
        P1IFG &= ~TOBC_PIN;	 // Clear default interrupt flag
    }

    __bic_SR_register_on_exit(LPM3_bits);  // Wake up
}
