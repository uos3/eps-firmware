/*
 * @ingroup gpio
 *
 * @file Interrupt_public.h
 * @author Daniel Murphy (dm4g16@soton.ac.uk)
 * @brief Public header file for interrupt driver.
 *
 * @version 0.1
 * @date 2021-03-30
 *
 * @copyright UOS3
 */

#ifndef DRIVERS_GPIO_INTERRUPT_INTERRUPT_PUBLIC_H_
#define DRIVERS_GPIO_INTERRUPT_INTERRUPT_PUBLIC_H_



/* -------------------------------------------------------------------------
 * INCLUDES
 * ------------------------------------------------------------------------- */

/* Standard library includes */
#include <stdio.h>
#include <stdint.h>
#include <msp430.h>



/* -------------------------------------------------------------------------
 * FUNCTIONS
 * ------------------------------------------------------------------------- */

void Intpin_init(void);
void Intpin_set(uint8_t interruptActive);



#endif /* DRIVERS_GPIO_INTERRUPT_INTERRUPT_PUBLIC_H_ */
