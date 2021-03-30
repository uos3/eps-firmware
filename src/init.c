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

	/* Set RX and TX pins to support UART */
	P3SEL |= RX_PIN | TX_PIN;

	/* Configure rest of UART (will be changed depending on full implementation */
	UCA0CTL1 |= UCSSEL_2;  // SMCLK
	UCA0BR0 = 0x08;	 // 1MHz 115200
	UCA0BR1 = 0x00;	 // 1MHz 115200
	UCA0MCTL = UCBRS2 + UCBRS0;	 // Modulation UCBRSx = 5
	UCA0CTL1 &= ~UCSWRST;  // **Initialize USCI state machine**
	UC0IE |= UCA0RXIE;	// Enable USCI_A0 RX interrupt

	/* Watchdog */
	WDTCTL = WDT_ADLY_1000;	 // Configure watchdog
	IE1 |= WDTIE;  // Enable watchdog

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
