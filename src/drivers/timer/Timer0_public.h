/*
 * @ingroup timer
 *
 * @file Timer0_public.h
 * @author Daniel Murphy (dm4g16@soton.ac.uk)
 * @brief Public header file for timer0 driver.
 *
 * @version 0.1
 * @date 2021-04-04
 *
 * @copyright UOS3
 */

#ifndef DRIVERS_TIMER_TIMER0_PUBLIC_H_
#define DRIVERS_TIMER_TIMER0_PUBLIC_H_

/* Standard library includes */
#include <stdio.h>
#include <stdint.h>
#include <msp430.h>

/* -------------------------------------------------------------------------
 * GLOBALS
 * ------------------------------------------------------------------------- */
static uint32_t timeleft = 0;
static void (*p_Timer0_callback_function_stored)();

/* -------------------------------------------------------------------------
 * FUNCTIONS
 * ------------------------------------------------------------------------- */
void Timer0_disable(void);
void Timer0_process(void);
void Timer0_start(uint32_t stopvalue, void (*p_Timer0_callback_function)());





#endif /* DRIVERS_TIMER_TIMER0_PUBLIC_H_ */
