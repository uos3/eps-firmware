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
void I2c_master_init(void) {
    /*Sets USCI registers while UCSWRST = 1*/
    UCB0CTL1 = UCSWRST;
    /*UCMST sets master mode, UCMODE_3 sets I2c mode and UCSYNC sets as synchronous (I2C)*/
    UCB0CTL0 = UCMST + UCMODE_3 + UCSYNC;
    /*Sets clock source, mode and keeps UCSWRST high*/
    UCB0CTL1 = UCSSEL_2 + UCSWRST;
    /*1MHz/10 = 100kHz, standard for I2C*/
    UCB0BR0 = 10;
    UCB0BR1 = 0;
    /*Sets UCSWRST = 0 ready to go*/
    UCB0CTL1 &= ~UCSWRST;
    /* Configures P3.1 and P3.2 as SDA and SCL*/
    /*P3SEL |= BIT1 + BIT2;
     * P3OUT |= BIT1 + BIT2;*/
    /*Sets SDA and SCL for launchpad */
    P1SEL |= BIT6 + BIT7;
    P1SEL |= BIT6 + BIT7;
}

uint8_t I2c_master_read(uint8_t slaveaddress_in, uint8_t bytecount_in,
                        uint8_t *p_data_out) {
    int i;
    int err = 0;
    uint16_t masterrxindex;
    /*Set the slave address */
    UCB0I2CSA = slaveaddress_in;
    /*Set into receive mode and send the start */
    UCB0CTL1 &= ~UCTR;
    UCB0CTL1 |= UCTXSTT;
    for (i = 0; i < 100;) {
        if (UCB0CTL1 & UCTXSTT) {
            i++;
        }
        if (i == 100) {
            return ERROR_START_NOT_RECEIVED;
        }
    }
    /*If there is only one byte to receive, the stop bit should be set immediately */
    if (bytecount_in == 1) {
        UCB0CTL1 |= UCTXSTP;
    }
    /*Check for ACK*/
    err = I2c_check_ack(slaveaddress_in);

    if (bytecount_in > 1) {
        masterrxindex = bytecount_in;
        for (i = masterrxindex; i > 0; i--) {
            if (err == 0) {
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
    return err;
}

uint8_t I2c_master_write(uint8_t slaveaddress_in, uint8_t bytecount_in,
                         uint8_t *p_data_in) {
    int err = 0;
    mastertxdata = p_data_in;
    uint16_t mastertxindex;
    /*Set slave address*/
    UCB0I2CSA = slaveaddress_in;
    /*Send the start condition and put in transmit mode*/
    UCB0CTL1 |= UCTR + UCTXSTT;
    /*Wait for start to be sent and ready to transmit*/
    while((UCB0CTL1 & UCTXSTT) && ((IFG2 & UCB0TXIFG) == 0));
   /* for (i = 0; i < 100; i++) {
        if ((IFG2 & UCB0TXIFG) == 0) {
        }
    }
    if (i == 100) {
        return ERROR_START_NOT_RECEIVED;
    }
    */
    /*Check for ACK*/
    err = I2c_check_ack(slaveaddress_in);
    /*Enable TX interrupt */
    if (err == 0) {
        for (mastertxindex = bytecount_in; mastertxindex > 0; mastertxindex--) {
            if (mastertxindex == 1) {
                UCB0TXBUF = *mastertxdata;
                UCB0CTL1 |= UCTXSTP;
            }
            else {
                UCB0TXBUF = *mastertxdata;
                mastertxdata++;
            }
        }
    }
    UCB0CTL1 |= UCTXSTT;
    return err;

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

