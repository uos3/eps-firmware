/*
 * uart.c
 *
 *  Created on: 9 Oct 2015
 *      Author: Tom
 */

#include <msp430.h>
#include <stdint.h>
#include "../util/circ.h"
#include "../core.h"

static circ rxBuffer;

void uart_init()
{
	circ_init(&rxBuffer);

	P3SEL |= BIT4 | BIT5;
    UCA0CTL1 |= UCSSEL_2; // Use SMCLK
    UCA0BR0 = (F_CPU / 57600UL) & 0xFF; // Set baud rate to 9600
    UCA0BR1 = ((F_CPU / 57600UL) >> 8) & 0xFF; // Set baud rate to 9600
    UCA0MCTL = UCBRS0; // Modulation UCBRSx = 1
    UCA0CTL1 &= ~UCSWRST; // Initialize USCI state machine
    IE2 |= UCA0RXIE; // Enable USCI_A0 RX interrupt
}

void uart_tx(uint8_t *buffer, uint8_t offset, uint8_t count)
{
	// TODO: Make interrupt driven.
	uint8_t i;
	for (i = 0; i < count; i++)
	{
		while  (!(IFG2&UCA0TXIFG));
		UCA0TXBUF = buffer[offset + i];
	}
}

uint8_t uart_rx(uint8_t *buffer, uint8_t offset, uint8_t count)
{
	uint8_t i;
	for (i = 0; i < count; i++)
	{
		int val = circ_pop(&rxBuffer);
		if (val == -1)
			break;
		buffer[offset + i] = (uint8_t)val;
	}
	return i;
}

void uart_handle_rx_interrupt(){
    circ_push(&rxBuffer, UCA0RXBUF);
}


//#pragma vector=USCIAB0RX_VECTOR
//__interrupt void USCIAB0RX_ISR(void)
//{
//    circ_push(&rxBuffer, UCA0RXBUF);
//    core_check_wakeup(UART);
//}
