/*
 *
 * @file BatteryComms_public.h
 * @author Roberto Holmes (rh3u17@soton.ac.uk / robertoholmes@live.com)
 * @brief Handle requests to communicate with the battery.
 *
 * @version 0.1
 * @date 2021-04-02
 *
 * @copyright UOS3
 */

#ifndef APPLICATIONS_BATTERY_COMMS_BATTERYCOMMS_PUBLIC_H_
#define APPLICATIONS_BATTERY_COMMS_BATTERYCOMMS_PUBLIC_H_

/* -------------------------------------------------------------------------
 * INCLUDES
 * ------------------------------------------------------------------------- */

/* Standard Library */
#include <stdint.h>

/* Internal */

/* -------------------------------------------------------------------------
 * DEFINES
 * ------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------
 * GLOBALS
 * ------------------------------------------------------------------------- */

#define HOUSE_KEEPING_DATA_LENGTH (20)

/* -------------------------------------------------------------------------
 * STRUCTS
 * ------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------
 * FUNCTIONS
 * ------------------------------------------------------------------------- */

uint16_t BatteryComms_comm(uint16_t command);

#endif /* APPLICATIONS_BATTERY_COMMS_BATTERYCOMMS_PUBLIC_H_ */
