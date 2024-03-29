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
#include "event_handlers/tobc_pin/TobcPin_public.h"
#include "event_handlers/tobc_watchdog/TobcWatchdog_public.h"
#include "event_handlers/serial_event/SerialEvent_public.h"

/* Applications */
#include "applications/log_file/LogFile_public.h"
#include "applications/rail_editor/RailEditor_public.h"

/* -------------------------------------------------------------------------
 * GLOBALS
 * ------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------
 * DEFINES
 * ------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------
 * STRUCTS
 * ------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------
 * FUNCTIONS
 * ------------------------------------------------------------------------- */

#endif /* MAIN_H_ */
