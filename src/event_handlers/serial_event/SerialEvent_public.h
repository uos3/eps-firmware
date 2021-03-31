/*
 *
 * @file Serial_public.h
 * @author Roberto Holmes (rh3u17@soton.ac.uk / robertoholmes@live.com)
 * @brief Decide what should be done if a serial RX interrupt has occurred.
 *
 * Task ref: [EPS_SW_1.4.1]
 *
 * @version 0.1
 * @date 2021-03-31
 *
 * @copyright UOS3
 */

#ifndef EVENT_HANDLERS_SERIAL_EVENT_SERIALEVENT_PUBLIC_H_
#define EVENT_HANDLERS_SERIAL_EVENT_SERIALEVENT_PUBLIC_H_

/* -------------------------------------------------------------------------
 * INCLUDES
 * ------------------------------------------------------------------------- */

/* Standard Library */
#include <msp430.h>

/* Internal */
#include "applications/serial_comms/SerialComms_public.h"

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

int SerialEvent_event();



#endif /* EVENT_HANDLERS_SERIAL_EVENT_SERIALEVENT_PUBLIC_H_ */
