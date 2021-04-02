/*
 *
 * @file Battery_public.h
 * @author Roberto Holmes (rh3u17@soton.ac.uk / robertoholmes@live.com)
 * @brief Hold functions to transmit to and receive from the battery.
 *
 * @version 0.1
 * @date 2021-04-02
 *
 * @copyright UOS3
 */

#ifndef COMPONENTS_BATTERY_BATTERY_PUBLIC_H_
#define COMPONENTS_BATTERY_BATTERY_PUBLIC_H_

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

/* -------------------------------------------------------------------------
 * STRUCTS
 * ------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------
 * FUNCTIONS
 * ------------------------------------------------------------------------- */

uint8_t Battery_TX(uint16_t command);

uint16_t Battery_RX();


#endif /* COMPONENTS_BATTERY_BATTERY_PUBLIC_H_ */
