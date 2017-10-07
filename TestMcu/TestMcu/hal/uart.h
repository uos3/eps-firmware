/*
 * uart.h
 *
 * Created: 27/10/2015 17:10:00
 *  Author: Tom
 */


#ifndef UART_H_
#define UART_H_

#include <stdint.h>

void uart_init();

uint8_t uart_pc_error();
uint8_t uart_pc_rx_len();
uint8_t uart_pc_rx_pop();
uint8_t uart_pc_rx_peek(uint8_t offset);
void uart_pc_tx_push(uint8_t val);

uint8_t uart_dut_error();
uint8_t uart_dut_rx_len();
uint8_t uart_dut_rx_pop();
uint8_t uart_dut_rx_peek(uint8_t offset);
void uart_dut_tx_push(uint8_t val);

#endif /* UART_H_ */