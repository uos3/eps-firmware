/*
 * pc.c
 *
 * Created: 28/10/2015 15:39:01
 *  Author: Tom
 */

#include <stdint.h>
#include <util/pipe.h>
#include <hal/uart.h>
#include <util/packet.h>
#include "error.h"
#include "dut.h"
#include "analogue.h"
#include "dac.h"

static PIPE pc_pipe = {
	&uart_pc_rx_len,
	&uart_pc_rx_pop,
	&uart_pc_rx_peek,
	&uart_pc_tx_push,
	&uart_pc_error
};

// Internal messages
static void pc_tx_ack();

static void pc_packet_process();
static uint8_t pc_get_packet_length();

static void send_int_state();

// Checks if there is enough data in the RX buffer to process a packet.
void pc_process()
{
	if (pc_pipe.length() != 0)
	{
		uint8_t pktLen;
		while (pktLen = pc_get_packet_length(), pc_pipe.length() >= pktLen)
		{
			pc_packet_process();
		}
	}
	
	uint8_t err = pc_pipe.error();
	if (err)
	{
		error_throw(CIRC_PC_ERROR_NONE | (err & 0x3));
	}
}

// Calculates the length of the packet in the receive buffer, or
// returns 0xFF if the packet has not been received yet.
static uint8_t pc_get_packet_length()
{
	return pc_pipe.length() > 1 ? 2 + pc_pipe.peek(1) : 0xFF;
}

static void handle_relay_power(uint8_t len)
{
	dut_tx(&pc_pipe, len);
}


static void dac_handle()
{
	uint8_t index = pc_pipe.pop();
	uint8_t value = pc_pipe.pop();
	dac_set(index, value);
}

// Processes the current packet in the receive buffer.
static void pc_packet_process()
{
	pc_tx_ack();
	
	uint8_t header = pc_pipe.pop();
	uint8_t len = pc_pipe.pop();
	switch(header)
	{
		case RELAY_POWER:
			handle_relay_power(len);
			break;
		case SET:
			dac_handle();
			break;
		case MEASURE:
			analogue_convert();
			break;
		case REQUEST_INT:
			send_int_state();
			break;
		default:
		{
			for(uint8_t i = 0; i < len; i++) pc_pipe.pop();
			error_throw(PC_UNKNOWN_PACKET);
			break;
		}
	}
}

static void send_int_state()
{
	pc_tx_relay_int(intpin_get());
}

static void pc_tx_ack()
{
	pc_pipe.push(ACK);
	pc_pipe.push(0);
}

void pc_tx_relay_host(PIPE *src, uint8_t len)
{
	pc_pipe.push(RELAY_HOST);
	pc_pipe.push(len);
	while(len--) pc_pipe.push(src->pop());
}

void pc_tx_error(ERR_CODE code)
{
	pc_pipe.push(ERROR);
	pc_pipe.push(1);
	pc_pipe.push(code);
}

void pc_tx_relay_int(uint8_t status)
{
	pc_pipe.push(RELAY_INT);
	pc_pipe.push(1);
	pc_pipe.push(status);
}

void pc_tx_data(uint16_t *data)
{
	pc_pipe.push(MEASUREMENT);
	pc_pipe.push(16);
	for (uint8_t i = 0; i < 8; i++)
	{
		pc_pipe.push(data[i] & 0xFF);
		pc_pipe.push((data[i] >> 8) & 0xFF);
	}
}

void pc_tx_ocp(uint32_t ticks)
{
	pc_pipe.push(OCP);
	pc_pipe.push(4);
	pc_pipe.push(ticks & 0xFF);
	pc_pipe.push((ticks>>8) & 0xFF);
	pc_pipe.push((ticks>>16) & 0xFF);
	pc_pipe.push((ticks>>24) & 0xFF);
}
