/*
 * dut.c
 *
 * Created: 28/10/2015 15:44:56
 *  Author: Tom
 */

#include <stdint.h>
#include <util/pipe.h>
#include <util/packet.h>
#include <hal/uart.h>
#include <hal/intpin.h>
#include "error.h"
#include "pc.h"

static PIPE dut_pipe = {
	&uart_dut_rx_len,
	&uart_dut_rx_pop,
	&uart_dut_rx_peek,
	&uart_dut_tx_push,
	&uart_dut_error
};


void dut_process()
{
	uint8_t recvBytes = dut_pipe.length();
	if (recvBytes != 0)
	{
		pc_tx_relay_host(&dut_pipe, recvBytes);
	}
	
	uint8_t err = dut_pipe.error();
	if (err)
	{
		error_throw(CIRC_DUT_ERROR_NONE | (err & 0x3));
	}
	
	if (intpin_get_changed())
	{
		static uint8_t lastPin = 0xFF;
		// Must be this way around, otherwise a transition could be missed.
		intpin_clear_changed();
		uint8_t pin = intpin_get();
		if (lastPin != pin)
		{
			lastPin = pin;
			pc_tx_relay_int(pin);
		}
	}
}

void dut_tx(PIPE *src, uint8_t len)
{
	while(len--) dut_pipe.push(src->pop());
}
