/*
 *
 * @file OCP_public.h
 * @author Roberto Holmes (rh3u17@soton.ac.uk / robertoholmes@live.com)
 * @brief Public Over Current Protection event handler header file
 *
 * Task ref: [EPS_SW_1.4.3]
 *
 * @version 0.1
 * @date 2021-03-31
 *
 * @copyright UOS3
 */

#ifndef EVENT_HANDLERS_OCP_OCP_PUBLIC_H_
#define EVENT_HANDLERS_OCP_OCP_PUBLIC_H_

/* -------------------------------------------------------------------------
 * INCLUDES
 * ------------------------------------------------------------------------- */

/* Standard Library */
#include <stdint.h>

/* Internal */
#include "applications/rail_editor/RailEditor_public.h"
#include "applications/config_file/ConfigFile_public.h"
#include "applications/log_file/LogFile_public.h"
#include "components/serial/Serial_public.h"

/* -------------------------------------------------------------------------
 * DEFINES
 * ------------------------------------------------------------------------- */

#define OCP_PACKET_SIZE (2)

/* -------------------------------------------------------------------------
 * GLOBALS
 * ------------------------------------------------------------------------- */

uint8_t OCP_PACKET [OCP_PACKET_SIZE];

/* -------------------------------------------------------------------------
 * STRUCTS
 * ------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------
 * FUNCTIONS
 * ------------------------------------------------------------------------- */

int Ocp_event(uint8_t rail_mask);

#endif /* EVENT_HANDLERS_OCP_OCP_PUBLIC_H_ */
