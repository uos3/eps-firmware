/*
 *
 * @file TobcWatchdog_public.h
 * @author Roberto Holmes (rh3u17@soton.ac.uk / robertoholmes@live.com)
 * @brief Header file for TobcWatchdog_public.c
 *
 * @version 0.1
 * @date 2021-03-31
 *
 * @copyright UOS3
 */

#ifndef EVENT_HANDLERS_TOBC_WATCHDOG_TOBCWATCHDOG_PUBLIC_H_
#define EVENT_HANDLERS_TOBC_WATCHDOG_TOBCWATCHDOG_PUBLIC_H_

/* -------------------------------------------------------------------------
 * INCLUDES
 * ------------------------------------------------------------------------- */

/* Standard Library */
#include <stdint.h>

/* Internal */
#include "applications/rail_editor/RailEditor_public.h"
#include "applications/interrupt_manager/InterruptManager_public.h"
#include "applications/log_file/LogFile_public.h"

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

int TobcWatchdog_event();

#endif /* EVENT_HANDLERS_TOBC_WATCHDOG_TOBCWATCHDOG_PUBLIC_H_ */
