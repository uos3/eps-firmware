/*
 *
 * @file main.h
 * @author Roberto Holmes (rh3u17@soton.ac.uk / robertoholmes@live.com)
 * @brief header file for EPS firmware main.
 *
 * @version 0.1
 * @date 2021-03-29
 *
 * @copyright UOS3
 */

#ifndef MAIN_H_
#define MAIN_H_

/* -------------------------------------------------------------------------
 * INCLUDES
 * ------------------------------------------------------------------------- */

/* Standard library includes */
#include <msp430.h>
#include <stdint.h>

/* Internal includes */
#include "init.h"

/* Event Handlers */
#include "event_handlers/ocp/Ocp_public.h"
#include "event_handlers/serial/Serial_public.h"
#include "event_handlers/tobc_pin/TobcPin_public.h"
#include "event_handlers/tobc_watchdog/TobcWatchdog_public.h"

/* Applications */
#include "applications/log_file/LogFile_public.h"

/* -------------------------------------------------------------------------
 * GLOBALS
 * ------------------------------------------------------------------------- */

uint16_t interrupt_flags = 0;
uint8_t modified_log = 0;

/* -------------------------------------------------------------------------
 * DEFINES
 * ------------------------------------------------------------------------- */

#define UART_FLAG BIT0
#define WATCHDOG_FLAG BIT1
#define TOBC_PIN_FLAG BIT2
#define OCP_FLAG BIT3

/* Pin number on the P1 bank */
#define OCP1_PIN BIT2
#define OCP2_PIN BIT1
#define OCP3_PIN BIT0
#define OCP4_PIN BIT5
#define OCP5_PIN BIT4
#define OCP6_PIN BIT3
#define TOBC_PIN BIT3

/* Pin number on the P1 bank */
#define OCP1_FLAG BIT4
#define OCP2_FLAG BIT5
#define OCP3_FLAG BIT6
#define OCP4_FLAG BIT7
#define OCP5_FLAG BIT8
#define OCP6_FLAG BIT9

/* -------------------------------------------------------------------------
 * STRUCTS
 * ------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------
 * FUNCTIONS
 * ------------------------------------------------------------------------- */

#endif /* MAIN_H_ */
