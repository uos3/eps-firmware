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

int main(void)
{
	/* ---- INITIALISATION ---- */

	/* Do msp430 initialisation */
	init_msp430();

	/* Do drivers init */
	init_drivers();

	/* Do component init */
	init_components();

	/* Do application init */
	init_applications();

	/* Check for startup reason */
	init_startup();

	P1DIR &= ~(TOBC_PIN | OCP1_PIN | OCP2_PIN | OCP3_PIN | OCP4_PIN | OCP5_PIN | OCP6_PIN);	 // Configure interrupts as input
	P1REN |= TOBC_PIN | OCP1_PIN | OCP2_PIN | OCP3_PIN | OCP4_PIN | OCP5_PIN | OCP6_PIN;  // Pull down if P1OUT.X = 0
	P1IE |= TOBC_PIN | OCP1_PIN | OCP2_PIN | OCP3_PIN | OCP4_PIN | OCP5_PIN | OCP6_PIN;	 // Enable pin interrupts
	P1IES |= TOBC_PIN | OCP1_PIN | OCP2_PIN | OCP3_PIN | OCP4_PIN | OCP5_PIN | OCP6_PIN;  // Falling edge if 1
	P1IFG &= ~(TOBC_PIN | OCP1_PIN | OCP2_PIN | OCP3_PIN | OCP4_PIN | OCP5_PIN | OCP6_PIN);	 // Clear flags

	/* ---- MAIN LOOP ---- */
	while (1)
	{
		/* For each flag check whether it is active and should be dealt with */

		/* Check for UART interrupt */
		if (interrupt_flags & UART_FLAG != 0)
		{
			Serial_event();
		}

		/* Check for watchdog timer interrupt */
		if (interrupt_flags & WATCHDOG_FLAG != 0)
		{
			TobcWatchdog_event();
		}

		/* Check for OCP interrupt */
		if (interrupt_flags & OCP_FLAG != 0)
		{
			Ocp_event();
		}

		/* Check for OBC pin interrupt */
		if (interrupt_flags & TOBC_PIN_FLAG != 0)
		{
			TobcPin_event();
		}

		/* If everything is dealt with and the log has not been modified, go to sleep (in LPM3) with interrupts enabled */
		if (interrupt_flags == 0)
		{
			if (modified_log)
			{
				LogFile_commit();
			}
			/* Go to sleep and enable listening to interrupts */
			__bis_SR_register(LPM3_bits | GIE);
		}
	}
}

/* Watchdog Timer interrupt service routine */
#pragma vector = WDT_VECTOR
__interrupt void WDT_ISR(void)
{
	interrupt_flags |= WATCHDOG_FLAG;  // Set custom interrupt flag
	__bic_SR_register_on_exit(LPM3_bits);  // Wake up
}

/* UART RX interrupt service routine */
#pragma vector = USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void)
{
	interrupt_flags |= UART_FLAG;  // Set custom interrupt flag for OCP event and specific rail
	__bic_SR_register_on_exit(LPM3_bits);  // Wake up
}

/* Port 1 interrupt service routine */
#pragma vector = PORT1_VECTOR
__interrupt void Port_1(void)
{
	/* OCP Interrupts */
	if (P1IFG & OCP1_PIN != 0)
	{
		interrupt_flags |= OCP_FLAG | OCP1_FLAG;  // Set custom interrupt flag for OCP event and specific rail
		P1IFG &= ~OCP1_PIN;	 // Clear default interrupt flag
	}
	if (P1IFG & OCP2_PIN != 0)
	{
		interrupt_flags |= OCP_FLAG | OCP2_FLAG;
		P1IFG &= ~OCP2_PIN;
	}
	if (P1IFG & OCP3_PIN != 0)
	{
		interrupt_flags |= OCP_FLAG | OCP3_FLAG;
		P1IFG &= ~OCP3_PIN;
	}
	if (P1IFG & OCP4_PIN != 0)
	{
		interrupt_flags |= OCP_FLAG | OCP4_FLAG;
		P1IFG &= ~OCP4_PIN;
	}
	if (P1IFG & OCP5_PIN != 0)
	{
		interrupt_flags |= OCP_FLAG | OCP5_FLAG;
		P1IFG &= ~OCP5_PIN;
	}
	if (P1IFG & OCP6_PIN != 0)
	{
		interrupt_flags |= OCP_FLAG | OCP6_FLAG;
		P1IFG &= ~OCP6_PIN;
	}

	/* TOBC Pin interrupt */
	if (P1IFG & TOBC_PIN != 0)
	{
		interrupt_flags |= TOBC_PIN_FLAG;  // Set custom interrupt flag
		P1IFG &= ~TOBC_PIN;	 // Clear default interrupt flag
	}

	__bic_SR_register_on_exit(LPM3_bits);  // Wake up
}
