/*
 *
 * @file init.c
 * @author Roberto Holmes (rh3u17@soton.ac.uk / robertoholmes@live.com)
 * @brief Contain initialisation functions.
 *
 * @version 0.1
 * @date 2021-03-29
 *
 * @copyright UOS3
 */

#include "init.h"

/* Perform initialisations required for the MSP430 */
int init_msp430() {
    WDTCTL = WDTPW | WDTHOLD; /* stop watchdog timer */

    /* Required for FR2311 but not the EPS MCU */
    //    PM5CTL0 &= ~LOCKLPM5;                   // Disable the GPIO power-on default high-impedance mode to activate previously configured port settings
    /* Configure directions and output values for each pin in each pin bank including unused ones */
    P1DIR = 0;
    P1OUT = 0;

    P2DIR = 0;
    P2OUT = 0;

    P3DIR = 0;
    P3OUT = 0;

    /* Temp interrupt init */

    /* Calibrating and setting internal oscillator to 1MHz */
    DCOCTL = 0;	 // Select lowest DCOx and MODx settings
    BCSCTL1 = CALBC1_1MHZ;	// Set DCO
    DCOCTL = CALDCO_1MHZ;

    /* Watchdog */
    /* Configure and start watchdog by:
     *  - giving it the password
     *  - clearing the counter
     *  - using the ACLK clock which is the only one that stays on in LPM3
     *  - setting the interval to reset 0x01 for 4s*/
    /* TODO: decide on duration and if this should be
     * used as an EPS reset will cause all rails to turn on */
    WDTCTL = WDTPW + WDTCNTCL + WDTSSEL + WDTIS0;

    /* Timer for periodical waking up */
    TA0CCTL0 = CCIE;                              // enable interrupt for when reaching the TA0CCR0 condition
    TA0CCR0 = 32678;      // TODO: Decide length (divide by 32678 for time in s)
    TA0CTL = TASSEL_1 | MC_1;                     // ACLK, up mode

    /* TOBC Timer */
    TA1CCTL0 = CCIE;                              // enable interrupt for when reaching the TA1CCR0 condition
    TA1CCR0 = 65356;      // TODO: Decide length (divide by 32678 for time in s)
    TA1CTL = TASSEL_1 | MC_1;                     // ACLK, up mode

    /*
     uint8_t i;
     for(i=0; i< 10;i++){
     i=2*i;
     }*/

    return 0;
}

/* Loops through each driver to initialise it */
int init_drivers() {
    return 0;
}

/* Loops through each component to initialise it */
int init_components() {
    return 0;
}

/* Loops through each application to initialise it */
int init_applications() {
    return 0;
}

/* Checks to see what the reason for the startup was and saves in the log file */
int init_startup() {
    return 0;
}
