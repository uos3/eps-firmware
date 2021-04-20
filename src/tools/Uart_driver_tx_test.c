#include "msp430g2553.h"
#include <stdint.h>


#include "drivers/uart/Uart_public.h"

void main(void)
{
   /*Stop watchdog*/
   WDTCTL = WDTPW + WDTHOLD; // Stop WDT
   /*Calibrate clock for 8MHz*/
   DCOCTL = 0;
   BCSCTL1 = CALBC1_8MHZ;
   DCOCTL = CALDCO_8MHZ;
   /*Turn off P2 to save power */
   P2DIR |= 0xFF;
   P2OUT &= 0x00;
   /*Set LEDs as ouptut*/
   P1DIR |= RXLED + TXLED;
   P1OUT &= 0x00;
   /*Enable RX Interrupt*/
   IE2 |= UCA0RXIE;
   Uart_init();
   uint8_t test = 0x63;
   uint8_t *p_test_buffer_in;
   p_test_buffer_in = &test;
   Uart_send_bytes(&test, 1);
}

#pragma vector=USCIAB0TX_VECTOR
__interrupt void USCI0TX_ISR(void)
{
    }

#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void)
{
   /*Turn on green LED*/
   P1OUT |= RXLED;
   Uart_recv_bytes(p_buffer_out, 1);
   if(*p_buffer_out == 0x63) {
       /*Turn on red LED*/
       P1OUT |= TXLED;
   }
   /*Turn off green LED*/
   P1OUT &= ~RXLED;
   IFG2 &= ~UCA0RXIFG;
}
