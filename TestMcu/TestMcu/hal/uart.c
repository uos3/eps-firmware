/*
 * uart.c
 *
 * Created: 27/10/2015 17:09:51
 *  Author: Tom
 */

#include <util/circ.h>
#include <util/core.h>
#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>

static circ rxPcBuffer;
static circ rxDutBuffer;

#define PC_BAUD  57600UL
#define DUT_BAUD 57600UL

void uart_init()
{
	circ_init(&rxPcBuffer);
	circ_init(&rxDutBuffer);
	
	UBRR0H = ((F_CPU / (16 * PC_BAUD) - 1) >> 8) & 0xFF;
	UBRR0L = (F_CPU / (16 * PC_BAUD) - 1) & 0xFF;
	UCSR0B |= _BV(RXCIE0) // RX Complete Interrupt Enable
	        | _BV(RXEN0)  // Receiver Enable
	        | _BV(TXEN0); // Transmitter Enable
	
	UBRR1H = ((F_CPU / (16 * DUT_BAUD) - 1) >> 8) & 0xFF;
	UBRR1L = (F_CPU / (16 * DUT_BAUD) - 1) & 0xFF;
	UCSR1B |= _BV(RXCIE1) // RX Complete Interrupt Enable
	        | _BV(RXEN1)  // Receiver Enable
	        | _BV(TXEN1); // Transmitter Enable
}

void uart_pc_tx_push(uint8_t val)
{
	while (!(UCSR0A & _BV(UDRE0)));
	UDR0 = val;
}

uint8_t uart_pc_rx_len()
{
	return circ_length(&rxPcBuffer);
}

uint8_t uart_pc_rx_pop()
{
	return circ_pop(&rxPcBuffer);
}

uint8_t uart_pc_rx_peek(uint8_t offset)
{
	return circ_peek(&rxPcBuffer, offset);
}

uint8_t uart_pc_error()
{
	uint8_t err = circ_get_error(&rxPcBuffer);
	circ_clear_error(&rxPcBuffer);
	return err;
}

void uart_dut_tx_push(uint8_t val)
{
	while (!(UCSR1A & _BV(UDRE1)));
	UDR1 = val;
}

uint8_t uart_dut_rx_len()
{
	return circ_length(&rxDutBuffer);
}

uint8_t uart_dut_rx_pop()
{
	return circ_pop(&rxDutBuffer);
}

uint8_t uart_dut_rx_peek(uint8_t offset)
{
	return circ_peek(&rxDutBuffer, offset);
}

uint8_t uart_dut_error()
{
	uint8_t err = circ_get_error(&rxDutBuffer);
	circ_clear_error(&rxDutBuffer);
	return err;
}

ISR(USART0_RX_vect)
{
	circ_push(&rxPcBuffer, UDR0);
}

ISR(USART1_RX_vect)
{
	circ_push(&rxDutBuffer, UDR1);
}