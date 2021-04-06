/*
 * @ingroup gpio
 *
 * @file Mux_public.h
 * @author Daniel Murphy (dm4g16@soton.ac.uk)
 * @brief Public header for MUX driver.
 *
 * @version 0.1
 * @date 2021-03-29
 *
 * @copyright UOS3
 */

#ifndef DRIVERS_GPIO_MUX_MUX_PUBLIC_H_
#define DRIVERS_GPIO_MUX_MUX_PUBLIC_H_

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
#define MUX_PINS (0x2F)
/* @brief MUX channel selected was outside of the allowed range*/
#define MUX_CHANNEL_OUTSIDE_RANGE (0x01)


/* -------------------------------------------------------------------------
 * FUNCTIONS
 * ------------------------------------------------------------------------- */

void Mux_init(void);
uint8_t Mux_select(uint8_t channel);

#endif /* DRIVERS_GPIO_MUX_MUX_PUBLIC_H_ */
