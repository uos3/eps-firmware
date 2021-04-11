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

#include "Interrupts_public.h"

void Interrupts_init() {
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
    /* Get watchdog interval from config */
    ConfigFile_read(CONFIG_FILE_WDT_INTERVAL, INTERRUPTS_8BIT_INTERVAL);

    /* Convert the interval stored in the 8bit array to a 16bit variable */
    INTERRUPTS_INTERVAL = INTERRUPTS_8BIT_INTERVAL[0] << 8
            | INTERRUPTS_8BIT_INTERVAL[1];

    /* If it is not a valid value, set it the default
     * (WDTIS1 for 64s, WDTIS0 for 4s) */
    if (INTERRUPTS_INTERVAL != 0 && INTERRUPTS_INTERVAL != 1
            && INTERRUPTS_INTERVAL != 2 && INTERRUPTS_INTERVAL != 3) {
        INTERRUPTS_INTERVAL = WDTIS1;
    }
    /* Configure and start watchdog by:
     *  - giving it the password
     *  - clearing the counter
     *  - using the ACLK clock which is the only one that stays on in LPM3
     *  - setting the interval to reset (0x01 for 4s) */
    /* TODO: decide on duration and if this should be
     * used as an EPS reset will cause all rails to turn on */
    WDTCTL = WDTPW | WDTCNTCL | WDTSSEL | INTERRUPTS_INTERVAL;

    /* ---- Timer for periodical waking up (Timer A_0) ---- */
    /* Get timer interval from config */
    ConfigFile_read(CONFIG_FILE_WAKE_TIMER, INTERRUPTS_8BIT_INTERVAL);

    /* Convert the interval stored in the 8bit array to a 16bit variable */
    INTERRUPTS_INTERVAL = INTERRUPTS_8BIT_INTERVAL[0] << 8
            | INTERRUPTS_8BIT_INTERVAL[1];
    /* If it is invalid, set it to default */
    if (INTERRUPTS_INTERVAL == 0 || INTERRUPTS_INTERVAL == 0xFFFF) {
        INTERRUPTS_INTERVAL = 65356;
    }
    /* Enable interrupt for when reaching the TA0CCR0 condition */
    TA0CCTL0 = CCIE;
    /* Set length (divide by 32678 for time in s) */
    TA0CCR0 = INTERRUPTS_INTERVAL;
    /* ACLK, up mode */
    TA0CTL = TASSEL_1 | MC_1;

    /* ---- TOBC Timer (Timer A_1) ---- */
    /* Get timer interval from config */
    ConfigFile_read(CONFIG_FILE_TOBC_TIMER, INTERRUPTS_8BIT_INTERVAL);
    /* Convert the interval stored in the 8bit array to a 16bit variable */
    INTERRUPTS_INTERVAL = INTERRUPTS_8BIT_INTERVAL[0] << 8
            | INTERRUPTS_8BIT_INTERVAL[1];
    /* If it is invalid, set it to default */
    if (INTERRUPTS_INTERVAL == 0 || INTERRUPTS_INTERVAL == 0xFFFF) {
        INTERRUPTS_INTERVAL = 65356;
    }
    /* Enable interrupt for when reaching the TA1CCR0 condition */
    TA1CCTL0 = CCIE;
    /* Set length (divide by 32678 for time in s) */
    TA1CCR0 = INTERRUPTS_INTERVAL;
    /* ACLK, up mode */
    TA1CTL = TASSEL_1 | MC_1;
}

/* Port 1 interrupt service routine */
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

    __bic_SR_register_on_exit(LPM3_bits);  // Wake up
}

/* Interrupt service routine to periodically wake up the EPS by leaving
 * Low Power Mode 3 */
#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer_A_CCR0_ISR(void) {
    __bic_SR_register_on_exit(LPM3_bits);
}

/* Interrupt service routine to reset the TOBC by setting
 * the custom flag and then waking up */
#pragma vector = TIMER1_A0_VECTOR
__interrupt void Timer_A_CCR1_ISR(void) {
    INTERRUPTS_FLAGS |= INTERRUPTS_WATCHDOG_FLAG;
    __bic_SR_register_on_exit(LPM3_bits);
}

/* UART RX interrupt service routine */
#pragma vector = USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void) {
    INTERRUPTS_FLAGS |= INTERRUPTS_UART_FLAG;
    __bic_SR_register_on_exit(LPM3_bits);
}
