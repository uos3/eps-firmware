#include <stdint.h>
#include <msp430.h>

#include "drivers/uart/Uart_public.h"

//#define TXLED BIT0
//#define RXLED BIT6
//#define TXD BIT2
//#define RXD BIT1

//#define TXD BIT4
//#define RXD BIT5

uint8_t p_buffer_out[30];
uint8_t return_value;



void main(void) {

    /*Stop watchdog*/
    WDTCTL = WDTPW + WDTHOLD; // Stop WDT
    /*Calibrate clock for 1MHz*/
    DCOCTL = 0;
    BCSCTL1 = CALBC1_1MHZ;
    DCOCTL = CALDCO_1MHZ;
    /*Turn off P2 to save power */
    P2DIR |= 0xFF;
    P2OUT &= 0x00;
    /*Set LEDs as output*/
//   P1DIR |= RXLED + TXLED;
//   P1OUT &= 0x00;
    /*Enable RX Interrupt*/
    IE2 |= UCA0RXIE;
//    uint8_t test = 0x63;
//   p_test_buffer_in = &test;
    Uart_init();
    /*Sends the test byte*/
//   Uart_send_bytes(&test, 1);
    /*Enter LPM0 until interrupt*/
    __bis_SR_register(CPUOFF + GIE);
    while (1) {
    }
}

#pragma vector=USCIAB0TX_VECTOR
__interrupt void USCI0TX_ISR(void) {
}

#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void) {
    uint8_t i;
    /*Turn on green LED*/
//   P1OUT |= RXLED;
    return_value = Uart_recv_bytes(p_buffer_out, 2);
   if(*p_buffer_out == 0x63) {
       __no_operation();
    /*Turn on red LED*/
//       P1OUT |= TXLED;
   }
    /*Turn off green LED*/
//   P1OUT &= ~RXLED;
    IFG2 &= ~UCA0RXIFG;
}