/*
 * @ingroup i2c
 *
 * @file I2c_public.c
 * @author Daniel Murphy (dm4g16@soton.ac.uk)
 * @brief Public source file for the I2C driver
 *
 * Implements I2c functionality for the MSP4302132
 *
 * @version 0.1
 * @date 2021-04-01
 *
 * @copyright UOS3
 */

/* -------------------------------------------------------------------------
 * INCLUDES
 * ------------------------------------------------------------------------- */

/* Standard library includes */
#include <stdio.h>
#include <stdint.h>
#include <msp430.h>

#include "drivers/i2c/I2c_public.h"

/* -------------------------------------------------------------------------
 * DEFINES
 * ------------------------------------------------------------------------- */
/*Returned if start bit not cleared by h/w*/
#define ERROR_START_NOT_RECEIVED (0x01)
/*Returned if RX IFG not set (RX buffer empty)*/
#define ERROR_NO_DATA_TO_RECEIVE (0x02)

/* -------------------------------------------------------------------------
 * FUNCTIONS
 * ------------------------------------------------------------------------- */
void I2c_master_init(uint8_t slaveaddress_in) {
    P3SEL |= 0x06;
    /*Sets USCI registers while UCSWRST = 1*/
    UCB0CTL1 |= UCSWRST;
    /*UCMST sets master mode, UCMODE_3 sets I2c mode and UCSYNC sets as synchronous (I2C)*/
    UCB0CTL0 = UCMST + UCMODE_3 + UCSYNC;
    /*Sets clock source, mode and keeps UCSWRST high*/
    UCB0CTL1 = UCSSEL_2 + UCSWRST;
    /*1MHz/10 = 100kHz, standard for I2C*/
    UCB0BR0 = 10;
    UCB0BR1 = 0;
    /*Sets UCSWRST = 0 ready to go*/
    UCB0I2CSA = slaveaddress_in;
    UCB0CTL1 &= ~UCSWRST;
    /* Configures P3.1 and P3.2 as SDA and SCL*/
}

uint8_t I2c_master_read(uint8_t slaveaddress_in, uint8_t bytecount_in,
                        uint8_t *p_data_out) {
    int i, err;
    /*Set into receive mode and send the start */
    UCB0CTL1 &= ~UCTR;
    UCB0CTL1 |= UCTXSTT;
    masterrxindex = bytecount_in;
    if (masterrxindex == 1) {
        UCB0CTL1 |= UCTXSTP;
    }
    err = I2c_check_ack(slaveaddress_in);
    if (err == 0) {

        if (masterrxindex > 0) {
            for (i = 0; i < masterrxindex; i++) {
                /*Checks to see if data to receive */
                for (i = 0; i < 100;) {
                    if (IFG2 & UCB0RXIFG == 0) {
                        i++;
                    }
                    else {
                        break;
                    }
                }
                if (i == 100) {
                    return ERROR_NO_DATA_TO_RECEIVE;
                }
                *p_data_out = UCB0RXBUF;
                p_data_out++;
                if (masterrxindex == 1) {
                    UCB0CTL1 |= UCTXSTP;
                }
            }
        }
    }
    /*If there is only one byte to receive, the stop bit should be set immediately */
    return 0;
}

uint8_t I2c_master_write(uint8_t slaveaddress_in, uint8_t bytecount_in,
                         uint8_t *p_data_in) {
    mastertxindex = bytecount_in;
    mastertxdata = p_data_in;
    UCB0CTL1 |= UCTR + UCTXSTT;
    IE2 |= UCB0TXIE;
    return 0;
}

static int I2c_check_ack(uint8_t slaveaddress_in) {
    /*This function checks for ack, if it receives nack stops transaction clears IFG and returns -1*/
    int err = 0;
    if (UCB0STAT & UCNACKIFG) {
        UCB0CTL1 |= UCTXSTP;
        UCB0STAT &= ~UCNACKIFG;

        err = -1;
    }
    return err;
}

#pragma vector = USCIAB0TX_VECTOR
__interrupt void USCIAB0TX_ISR(void) {
    if (mastertxindex)                            // Check TX byte counter
    {
        UCB0TXBUF = *mastertxdata;                 // Load TX buffer
        mastertxdata++;
        mastertxindex--;                 // Decrement TX byte counter
    }
    else {
        UCB0CTL1 |= UCTXSTP;                   // I2C stop condition
        IFG2 &= ~UCB0TXIFG;
        __bic_SR_register_on_exit(CPUOFF);                   // Exit LPM0
    }
}

