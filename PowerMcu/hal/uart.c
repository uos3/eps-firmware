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
#include "hal/uart.h"



void uart_init()
{
	circ_init(&rxBuffer);

	//Recommended init procedure see pg413 http://www.ti.com/lit/ug/slau144j/slau144j.pdf
	UCA0CTL1 = UCSWRST;

	//Initialize all USCI registers while UCSWRST= 1
	UCA0CTL1 |= UCSSEL_2; // Use SMCLK which auto-activates when needed
    UCA0BR0 = (F_CPU / 56000UL) & 0xFF; // Set baud rate to 56000                           //UCBRx 833, UCBRS 2 UCBRF 0, divisor was 57600 previously, see typical baud rates and errors
    UCA0BR1 = ((F_CPU / 56000UL) >> 8) & 0xFF; // Set baud rate to 56000
    UCA0MCTL = 0x0E; //0x04;        //Value from recommended UCBRS and UCBRF

	P3SEL |= BIT4 | BIT5;   //Setting rx and tx functionality
	P3DIR |= BIT4;          //Setting to output
	P3DIR &= ~BIT5;         //Setting to input

	UCA0CTL1 &= ~UCSWRST;   //Ready to receive

	//Enable interrupts
    IE2 |= UCA0RXIE; // Enable USCI_A0 RX interrupt
        //UCA0TXIE for if transmit interrupt added

/*
	P3SEL |= BIT4 | BIT5;
    UCA0CTL1 |= UCSSEL_2; // Use SMCLK
    UCA0BR0 = (F_CPU / 57600UL) & 0xFF; // Set baud rate to 9600
    UCA0BR1 = ((F_CPU / 57600UL) >> 8) & 0xFF; // Set baud rate to 9600
    UCA0MCTL = UCBRS0; // Modulation UCBRSx = 1
    UCA0CTL1 &= ~UCSWRST; // Initialize USCI state machine
    IE2 |= UCA0RXIE; // Enable USCI_A0 RX interrupt
    */
}

void uart_tx(uint8_t *buffer, uint8_t count)
{
    //Count is hardcoded to 0 in TOBC this and the ISR would need to be changed if this was to change, ignore the power advice this is used in comm.c ISR
    packetsToSend = 6;
    txBufferAddress = buffer;
    IE2 |= UCA0TXIE;

/*	uint8_t i;
	for (i = 0; i < count; i++)
	{
		while  (!(IFG2&UCA0TXIFG));
		UCA0TXBUF = buffer[offset + i];
	}
*/

}

uint8_t uart_rx(uint8_t *buffer, uint8_t offset, uint8_t count)
{
	uint8_t i;
	for (i = 0; i < count; i++)
	{
		uint8_t val = circ_pop(&rxBuffer);
		if (val == 0xFF)
			break;
		buffer[offset + i] = (uint8_t)val;
	}
	return i;
}

inline void uart_handle_rx_interrupt(){
    circ_push(&rxBuffer, UCA0RXBUF);
}


//#pragma vector=USCIAB0RX_VECTOR
//__interrupt void USCIAB0RX_ISR(void)
//{
//    circ_push(&rxBuffer, UCA0RXBUF);
//    core_check_wakeup(UART);
//}

