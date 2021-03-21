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
#include "drivers/uart/Uart_errors.h"
#include "util/circular_buffers/Circular_buffer_public.h"
#include <msp430.h>

/* -------------------------------------------------------------------------
 * GLOBALS
 * ------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------
 * DEFINES
 * ------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------
 * STRUCTS
 * ------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------
 * FUNCTIONS
 * ------------------------------------------------------------------------- */

void Uart_init(void){
    /*Initialises the circular buffer used for RX*/
    Circular_buffer_init(&rxbuffer);
    /* Initialises the UART through the USCI*/
    /* Sets UCSWRST to 1*/
    UCA0CTL1 = UCSWRST;
    /*Initialise all USCI registers while UCSWRST = 1 */
    /*Use SMCLK*/
    UCA0CTL1 |= UCSSEL_2;
    /*Sets baud rate to 56000, 8MHz clock speed
     * calculated values using http://mspgcc.sourceforge.net/baudrate.html */
    /*TODO set clock speed as a global function in what document?*/
    UCA0BR0 = 0x8E;
    UCA0BR1 = 0x00;
    UCA0MCTL = 0xF7;

    /*Sets pins for RX (3.5) and TX(3.4)*/
    P3SEL |= BIT4 | BIT5;
    /*Sets TX as output */
    P3DIR |= BIT4;
    /*Sets RX as input */
    P3DIR &= ~BIT5;

    /*UCSWRST set back to 0 ready to receive */
    UCA0CTL1 &= ~UCSWRST;

    /* Enable Interrupts */
    IE2 |= UCA0RXIE; /* RX Interrupt */
    IE2 |= UCA0TXIE; /* TX Interrupt */

}

ErrorCode Uart_send_bytes(uint8_t *p_buffer, uint8_t offset, uint8_t count){
    /*Writes to the TX buffer to be sent over the UART interface */
    uint8_t i, j;
    for (i = 0; i < count; i ++){
        /*while(!(IFG2&UCA0TXIFG)){
        UCA0TXBUF = p_buffer[offset + i];*/
        for(j = 0; j < MAX_TRYS;){
            if(!(IFG2&UCA0TXIFG)){
                UCA0TXBUF = p_buffer[offset + i];
            }else{
                j ++;
        }
        }
    }
        if(j == MAX_TRYS){
            return UART_TX_ERROR_MAX_TRYS_REACHED;
        }else{
            return ERROR_NONE;
        }
    }


ErrorCode Uart_recv_bytes(uint8_t * p_buffer, uint8_t offset, uint8_t count){
    /* This function pushes the data from the RX buffer to a circular buffer, checks to see if the
     * buffer is correctly filled and then copies it to the buffer parameter on the input.*/
    uint8_t i;
    if (Circular_buffer_push(&rxbuffer, UCA0RXBUF) == 0){
        return UART_RX_ERROR_QUEUE_FULL;
    }else{
        for (i = 0; i < count; i ++){
                uint8_t val = Circular_buffer_pop(&rxbuffer);
                if(val == 0xFF){
                    return UART_RX_ERROR_QUEUE_EMPTY;
                }else{
                    p_buffer[offset + i] = val;
            }
        }
            return ERROR_NONE;
}
}




