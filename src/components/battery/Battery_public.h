/*
 *
 * @file Battery_public.h
 * @author Roberto Holmes (rh3u17@soton.ac.uk / robertoholmes@live.com)
 * @brief Hold functions to transmit to and receive from the battery.
 *
 * @version 0.1
 * @date 2021-04-09
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
#include "drivers/i2c/I2c_public.h"

/* -------------------------------------------------------------------------
 * DEFINES
 * ------------------------------------------------------------------------- */

#define BATTERY_I2C_ADDR (0x2A)
#define BATTERY_PACKET_MAX_LENGTH (3)
#define BATTERY_RX_PACKET_LENGTH (2)

/* -------------------------------------------------------------------------
 * GLOBALS
 * ------------------------------------------------------------------------- */

uint8_t BATTERY_PACKET[BATTERY_PACKET_MAX_LENGTH];

/* -------------------------------------------------------------------------
 * STRUCTS
 * ------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------
 * FUNCTIONS
 * ------------------------------------------------------------------------- */

uint8_t Battery_TX(uint8_t command_in, uint16_t data_in);

uint16_t Battery_RX();


#endif /* COMPONENTS_BATTERY_BATTERY_PUBLIC_H_ */
