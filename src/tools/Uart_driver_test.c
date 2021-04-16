#include <msp430.h>
#include "drivers/uart/Uart_public.h"

/* Standard Library */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

uint8_t *p_test_buffer_out;


int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	DCOCTL = 0;
	BCSCTL1 = CALBC1_8MHZ;
	DCOCTL = CALDCO_8MHZ;
	int i;
	/*Set pin 0(red) and 6(green) as outputs*/
	P1DIR = 0X41;
	/*Set LED to red*/
	P1OUT = 0x01;
	uint8_t test = 0xFF;
	uint8_t *p_test_buffer_in;
	p_test_buffer_in = &test;
	Uart_init();
	Uart_send_bytes(p_test_buffer_in, 1);
	for(i = 0; i < 3000; i ++) {
	    if(*p_test_buffer_in == *p_test_buffer_out) {
	        /*Turn LED green*/
	        P1OUT^=0x41;
	    } else {

	    }
	}
	

	return 0;
}


#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void)
{
    *p_test_buffer_out = UCA0RXBUF;
}
