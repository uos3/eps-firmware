/*
 * @ingroup uart
 *
 * @file Uart_public.c
 * @author Daniel Murphy (dm4g16@soton.ac.uk)
 * @brief Public UART driver
 *
 * Implements UART driver for the MSP4302132
 *
 * @version 0.1
 * @date 2021-03-15
 *
 * @copyright UOS3
 */

/* -------------------------------------------------------------------------
 * INCLUDES
 * ------------------------------------------------------------------------- */

/* Standard Library */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* Internal */
#include "drivers/uart/Uart_public.h"
#include <msp430.h>

/* -------------------------------------------------------------------------
 * FUNCTIONS
 * ------------------------------------------------------------------------- */
/* @brief Initialises the UART
 * Follows init procedure recommended pg413 http://www.ti.com/lit/ug/slau144j/slau144j.pdf
 * @return 0 if successful, non-zero if not
 */
void Uart_init(void) {

    /* Initialises the UART through the USCI*/
    /* Sets UCSWRST to 1*/
    UCA0CTL1 = UCSWRST;
    /*Initialise all USCI registers while UCSWRST = 1 */
    /*Use SMCLK*/
    UCA0CTL1 |= UCSSEL_2;
    /*Sets baud rate to 56000, 8MHz clock speed
     * calculated values using http://mspgcc.sourceforge.net/baudrate.html */
//    UCA0BR0 = (8000000 / 56000UL) & 0xFF;
//    UCA0BR1 = ((8000000 / 56000UL) >> 8) & 0xFF;
//    UCA0MCTL = 0x0E;
    /* Baud rate to 56000 for 1MHZ DCO clock speed */
    UCA0BR0 = (1000000 / 56000UL) & 0xFF;
    UCA0BR1 = ((1000000 / 56000UL) >> 8) & 0xFF;
    UCA0MCTL = 0x0E;

//    UCA0BR0 = (1000000 / 9600UL) & 0xFF;
//    UCA0BR1 = ((1000000 / 9600UL) >> 8) & 0xFF;
//    UCA0MCTL = UCBRS0;

    /*Sets pins for RX (3.5) and TX(3.4)*/
    P3SEL |= BIT4 | BIT5;
    /*Sets TX as output */
    P3DIR |= BIT4;
    /*Sets RX as input */
    P3DIR &= ~BIT5;
    /*different pinout for launchpad
     P1SEL |= BIT1 + BIT2;
     P1SEL2 |=  BIT1 + BIT2;
     P1DIR |= BIT2;
     P1DIR &= ~BIT1;
     */

    /*UCSWRST set back to 0 ready to receive */
    UCA0CTL1 &= ~UCSWRST;

    /* Enable Interrupts */
    IE2 |= UCA0RXIE; /* RX Interrupt */

}

uint8_t Uart_send_bytes(uint8_t *p_buffer_in, uint8_t length_in) {
    /*Writes content of p_buffer to TX buffer to be sent over the UART */
    uint8_t i;
    uint16_t j;
    for (i = 0; i < length_in; i++) {
        for (j = 0; j < MAX_TRYS; j++) {
            /* checks UCA0TXBUF empty */
            if (IFG2 & UCA0TXIFG) {
                UCA0TXBUF = p_buffer_in[i];
                break;
            }
        }
        if (j == MAX_TRYS) {
            __no_operation();
            return 1;
        }
    }
    return 0;
}

uint8_t Uart_recv_bytes(uint8_t *p_buffer_out, uint8_t length_in) {
    /* Checks the interrupt flag to see if a character has been received,
     * then reads and stores in p_buffer_out*/
    uint8_t i;
    uint16_t j;
    for (i = 0; i < length_in; i++) {
        for (j = 0; j < MAX_TRYS;j++) {
            if (IFG2 & UCA0RXIFG) {
                p_buffer_out[i] = UCA0RXBUF;
                break;
            }
        }
        if (j == MAX_TRYS) {
            return UART_RX_BUFFER_EMPTY_MAX_ATTEMPTS_REACHED;
        }
    }
    return 0;
}
