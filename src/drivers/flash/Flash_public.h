/*
 * @ingroup flash
 *
 * @file Flash_public.c
 * @author Daniel Murphy (dm4g16@soton.ac.uk)
 * @brief Public Flash driver source file
 *
 * Implements Flash driver for the MSP4302132
 *
 * @version 0.1
 * @date 2021-04-10
 *
 * @copyright UOS3
 */

#ifndef DRIVERS_FLASH_FLASH_PUBLIC_H_
#define DRIVERS_FLASH_FLASH_PUBLIC_H_

/* -------------------------------------------------------------------------
 * INCLUDES
 * ------------------------------------------------------------------------- */

/* Standard library includes */
#include <stdio.h>
#include <stdint.h>

/* Internal includes */
#include <msp430.h>

/* -------------------------------------------------------------------------
 * FUNCTIONS
 * ------------------------------------------------------------------------- */
uint8_t Flash_init(void);
uint8_t Flash_read(uint8_t address, uint8_t bytecount_in, uint8_t *p_data_out);
uint8_t Flash_write(uint8_t address, uint8_t bytecount_in, uint8_t *p_data_in);

#endif /* DRIVERS_FLASH_FLASH_PUBLIC_H_ */
