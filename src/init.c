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
void init_msp430() {
    /* Stop watchdog timer */
    WDTCTL = WDTPW | WDTHOLD;

    /* Configure directions and output values for each pin in each pin bank including unused ones */
    P1DIR = 0;
    P1OUT = 0;

    P2DIR = 0;
    P2OUT = 0;

    P3DIR = 0;
    P3OUT = 0;

    /* Calibrating and setting internal oscillator to 1MHz */
    DCOCTL = 0;
    BCSCTL1 = CALBC1_1MHZ;
    DCOCTL = CALDCO_1MHZ;
}

/* Checks to see what the reason for the startup was and saves in the log file */
void init_startup() {
    /* TODO: Read BOR, oscillator faults, and flash memory flags */
    IFG1 = 0;
    IFG2 = 0;
//    ACCVIFG;
    return;
}

/* Loops through each driver to initialise it */
void init_drivers() {
    Adc_init();
    Mux_init();
    Mux2_init();
    Uart_init();
    Flash_init();
}

/* Loops through each component to initialise it */
void init_components() {
    Rails_init();
}

/* Loops through each application to initialise it */
void init_applications() {
    LogFile_init();
}

/* Setup the interrupts */
void init_interrupts() {
    Interrupts_init();
}

