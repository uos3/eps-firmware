/*
 *
 * @file init.h
 * @author Roberto Holmes (rh3u17@soton.ac.uk / robertoholmes@live.com)
 * @brief Header file for initialisation functions.
 *
 * @version 0.1
 * @date 2021-03-29
 *
 * @copyright UOS3
 */

#ifndef INIT_H_
#define INIT_H_

/* -------------------------------------------------------------------------
 * INCLUDES
 * ------------------------------------------------------------------------- */
/* Standard library includes */
#include <msp430.h>

/* internal */
#include "applications/interrupt_manager/InterruptManager_public.h"
#include "applications/log_file/LogFile_public.h"

#include "drivers/uart/Uart_public.h"

/* -------------------------------------------------------------------------
 * GLOBALS
 * ------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------
 * DEFINES
 * ------------------------------------------------------------------------- */

#define RX_PIN BIT5
#define TX_PIN BIT4

/* -------------------------------------------------------------------------
 * STRUCTS
 * ------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------
 * FUNCTIONS
 * ------------------------------------------------------------------------- */
int init_msp430();
int init_drivers();
int init_components();
int init_applications();
int init_startup();

#endif /* INIT_H_ */
