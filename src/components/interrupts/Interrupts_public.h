/*
 *
 * @file Interrupts_public.h
 * @author Roberto Holmes (rh3u17@soton.ac.uk / robertoholmes@live.com)
 * @brief Public Interrupt component header file
 *
 * Task ref: [EPS_SW_1.6.4]
 *
 * @version 0.1
 * @date 2021-04-11
 *
 * @copyright UOS3
 */

#ifndef COMPONENTS_INTERRUPTS_INTERRUPTS_PUBLIC_H_
#define COMPONENTS_INTERRUPTS_INTERRUPTS_PUBLIC_H_

/* -------------------------------------------------------------------------
 * INCLUDES
 * ------------------------------------------------------------------------- */

/* Standard library includes */
#include <msp430.h>
#include <stdint.h>

/* Internal */
#include "components/rails/Rails_public.h"
#include "components/serial/Serial_public.h"

#include "applications/config_file/ConfigFile_public.h"

/* -------------------------------------------------------------------------
 * GLOBALS
 * ------------------------------------------------------------------------- */

uint16_t INTERRUPTS_FLAGS;

/* -------------------------------------------------------------------------
 * DEFINES
 * ------------------------------------------------------------------------- */

#define INTERRUPTS_UART_FLAG BIT0
#define INTERRUPTS_WATCHDOG_FLAG BIT1
#define INTERRUPTS_TOBC_INT_FLAG BIT2
#define INTERRUPTS_OCP_FLAG BIT3

/* Pin number on the P1 bank */
#define INTERRUPTS_OCP1_FLAG BIT4
#define INTERRUPTS_OCP2_FLAG BIT5
#define INTERRUPTS_OCP3_FLAG BIT6
#define INTERRUPTS_OCP4_FLAG BIT7
#define INTERRUPTS_OCP5_FLAG BIT8
#define INTERRUPTS_OCP6_FLAG BIT9

#define INTERRUPTS_TOBC_INT_PIN BIT3

/* -------------------------------------------------------------------------
 * STRUCTS
 * ------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------
 * FUNCTIONS
 * ------------------------------------------------------------------------- */
void Interrupts_init();

#endif /* COMPONENTS_INTERRUPTS_INTERRUPTS_PUBLIC_H_ */
