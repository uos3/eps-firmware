/*
 * uart.h
 *
 *  Created on: 9 Oct 2015
 *      Author: Tom
 */

#ifndef HAL_UART_H_
#define HAL_UART_H_

#include <stdint.h>
#include "../util/circ.h"

void uart_init();
//void uart_tx(char *str);
void uart_tx(uint8_t *buffer, uint8_t length);
uint8_t uart_rx(uint8_t *buffer, uint8_t offset, uint8_t length);
inline void uart_handle_rx_interrupt();

static uint8_t packetsToSend= 0;
static uint8_t *txBufferAddress;
static circ rxBuffer;

#endif /* HAL_UART_H_ */
