/*
 *
 * @file Interrupts_public.c
 * @author Roberto Holmes (rh3u17@soton.ac.uk / robertoholmes@live.com)
 * @brief Public Interrupt component source file
 *
 * Defines and initialises the main interrupts
 *
 * Task ref: [EPS_SW_1.6.4]
 *
 * @version 0.1
 * @date 2021-04-11
 *
 * @copyright UOS3
 */

/* -------------------------------------------------------------------------
 * INCLUDES
 * ------------------------------------------------------------------------- */

#include "Interrupts_public.h"

/* -------------------------------------------------------------------------
 * FUNCTIONS
 * ------------------------------------------------------------------------- */

void Interrupts_init() {
    uint16_t interval;
    INTERRUPTS_FLAGS = 0;

    /* Set TOBC interrupt pin as input */
    P1DIR &= ~INTERRUPTS_TOBC_INT_PIN;
    /* Enable pull down resistor */
    P1REN |= INTERRUPTS_TOBC_INT_PIN;
    /* Enable interrupt */
    P1IE |= INTERRUPTS_TOBC_INT_PIN;
    /* Set interrupt on rising edge */
    P1IES &= ~INTERRUPTS_TOBC_INT_PIN;
    /* Clear interrupt flag */
    P1IFG &= ~INTERRUPTS_TOBC_INT_PIN;

    /* ---- Watchdog ---- */
    /* Configure and start watchdog by:
     *  - giving it the password
     *  - clearing the counter
     *  - using the ACLK clock which is the only one that stays on in LPM3
     *  - setting the interval to 64s to reset (0x01 (WDTIS0) for 4s,
     *  0x10 (WDTIS1) for 64s) */
    /* TODO: decide if this should be used as an EPS reset
     *  will cause all rails to turn on */
    WDTCTL = WDTPW | WDTCNTCL | WDTSSEL | WDTIS1;

    /* ---- Timer for periodical waking up (Timer A_0) ---- */
    /* Enable interrupt for when reaching the TA0CCR0 condition */
    TA0CCTL0 = CCIE;
    /* Set length to 31s (divide by 2048 for time in s) */
    TA0CCR0 = 0xFFFF;
    /* ACLK, /8 input divider, up/down mode */
    TA0CTL = TASSEL_1 | ID_1 | ID_0 | MC_1 | MC_0;

    /* ---- TOBC Timer (Timer A_1) ---- */
    /* Get timer interval from config */
    ConfigFile_read_16bit(CONFIG_FILE_TOBC_TIMER, &interval);
    /* If it is invalid, set it to default (32s) */
    if (interval == 0) {
        interval = 0xFFFF;
    }
    /* Enable interrupt for when reaching the TA1CCR0 condition */
    TA1CCTL0 = CCIE;
    /* Set length (divide by 2048 for time in s) */
    TA1CCR0 = interval;
    /* ACLK, /8 input divider, up/down mode */
    TA1CTL = TASSEL_1 | ID_1 | ID_0 | MC_1 | MC_0;
}

/* Port 1 interrupt service routine (OCP events and TOBC resetting timer) */
#pragma vector = PORT1_VECTOR
__interrupt void Port_1(void) {

    /* OCP Interrupts */
    if (P1IFG & RAILS_OCP1_PIN != 0) {
        /* Set custom interrupt flag for OCP event and specific rail
         * and then clear the default interrupt flag */
        INTERRUPTS_FLAGS |= INTERRUPTS_OCP_FLAG | INTERRUPTS_OCP1_FLAG;
        P1IFG &= ~RAILS_OCP1_PIN;
    }
    if (P1IFG & RAILS_OCP2_PIN != 0) {
        INTERRUPTS_FLAGS |= INTERRUPTS_OCP_FLAG | INTERRUPTS_OCP2_FLAG;
        P1IFG &= ~RAILS_OCP2_PIN;
    }
    if (P1IFG & RAILS_OCP3_PIN != 0) {
        INTERRUPTS_FLAGS |= INTERRUPTS_OCP_FLAG | INTERRUPTS_OCP3_FLAG;
        P1IFG &= ~RAILS_OCP3_PIN;
    }
    if (P1IFG & RAILS_OCP4_PIN != 0) {
        INTERRUPTS_FLAGS |= INTERRUPTS_OCP_FLAG | INTERRUPTS_OCP4_FLAG;
        P1IFG &= ~RAILS_OCP4_PIN;
    }
    if (P1IFG & RAILS_OCP5_PIN != 0) {
        INTERRUPTS_FLAGS |= INTERRUPTS_OCP_FLAG | INTERRUPTS_OCP5_FLAG;
        P1IFG &= ~RAILS_OCP5_PIN;
    }
    if (P1IFG & RAILS_OCP6_PIN != 0) {
        INTERRUPTS_FLAGS |= INTERRUPTS_OCP_FLAG | INTERRUPTS_OCP6_FLAG;
        P1IFG &= ~RAILS_OCP6_PIN;
    }

    /* TOBC Pin interrupt */
    if (P1IFG & INTERRUPTS_TOBC_INT_PIN != 0) {
        /* Set custom interrupt flag and clear the default one */
        INTERRUPTS_FLAGS |= INTERRUPTS_TOBC_INT_FLAG;
        P1IFG &= ~INTERRUPTS_TOBC_INT_PIN;
    }
    /* Enable interrupts on wake (bit set status register
     * on exit Global Interrupt Enable) */
    __bis_SR_register_on_exit(GIE);
    /* Wake up (bit clear Status Register on exit Low Power Mode 3) */
    __bic_SR_register_on_exit(LPM3_bits);
}

/* Interrupt service routine to periodically wake up the EPS by leaving
 * Low Power Mode 3 */
#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer_A_CCR0_ISR(void) {
    __bis_SR_register_on_exit(GIE);
    __bic_SR_register_on_exit(LPM3_bits);
}

/* Interrupt service routine to reset the TOBC by setting
 * the custom flag and then waking up */
#pragma vector = TIMER1_A0_VECTOR
__interrupt void Timer_A_CCR1_ISR(void) {
    INTERRUPTS_FLAGS |= INTERRUPTS_WATCHDOG_FLAG;
    __bis_SR_register_on_exit(GIE);
    __bic_SR_register_on_exit(LPM3_bits);
}

/* UART RX interrupt service routine */
#pragma vector = USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void) {
    /* Collect arriving data into the buffer */
    Serial_process_RX();
    INTERRUPTS_FLAGS |= INTERRUPTS_UART_FLAG;
    __bis_SR_register_on_exit(GIE);
    __bic_SR_register_on_exit(LPM3_bits);
}
