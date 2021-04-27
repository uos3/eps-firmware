/*
 * @ingroup i2c
 *
 * @file I2c_public.h
 * @author Daniel Murphy (dm4g16@soton.ac.uk)
 * @brief Public header for the I2C driver
 *
 * Implements I2c functionality for the MSP430
 *
 * @version 0.1
 * @date 2021-04-01
 *
 * @copyright UOS3
 */

#ifndef DRIVERS_I2C_I2C_PUBLIC_H_
#define DRIVERS_I2C_I2C_PUBLIC_H_

/* -------------------------------------------------------------------------
 * INCLUDES
 * ------------------------------------------------------------------------- */
/* Standard library includes */
#include <stdio.h>
#include <stdint.h>
#include <msp430.h>

/* -------------------------------------------------------------------------
 * DEFINES
 * ------------------------------------------------------------------------- */
/*Returned if start bit not cleared by h/w*/
#define ERROR_START_NOT_RECEIVED (0x01)
/*Returned if RX IFG not set (RX buffer empty)*/
#define ERROR_NO_DATA_TO_RECEIVE (0x02)
/* -------------------------------------------------------------------------
 * GLOBALS
 * ------------------------------------------------------------------------- */
/*Buffer to store data received*/
static uint8_t *masterrxdata;
/*Buffer to store data to transmit*/
static uint8_t *mastertxdata;


/* -------------------------------------------------------------------------
 * FUNCTIONS
 * ------------------------------------------------------------------------- */

void I2c_master_init();
uint8_t I2c_master_read(uint8_t slaveaddress_in, uint8_t bytecount_in, uint8_t *p_data_out);
uint8_t I2c_master_write(uint8_t slaveaddress_in, uint8_t bytecount_in, uint8_t *p_data_in);
static int I2c_check_ack(uint8_t slaveaddress_in);

#endif /* DRIVERS_I2C_I2C_PUBLIC_H_ */
