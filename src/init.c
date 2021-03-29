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
int init_msp430()
{
    WDTCTL = WDTPW | WDTHOLD;   /* stop watchdog timer */

    /* Required for FR2311 but not the EPS MCU */
//    PM5CTL0 &= ~LOCKLPM5;                   // Disable the GPIO power-on default high-impedance mode
                                            // to activate previously configured port settings

    /* Configure directions and output values for each pin in each pin bank */
    P1DIR = 0;
    P1OUT = 0;

    P2DIR = 0;
    P2OUT = 0;

    return 0;
}

/* Loops through each driver to initialise it */
int init_drivers()
{
	return 0;
}

/* Loops through each component to initialise it */
int init_components()
{
	return 0;
}

/* Loops through each application to initialise it */
int init_applications()
{
	return 0;
}

/* Checks to see what the reason for the startup was and saves in the log file */
int init_startup()
{
    return 0;
}
