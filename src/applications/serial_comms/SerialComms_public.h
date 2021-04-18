/*
 *
 * @file SerialComms_public.h
 * @author Roberto Holmes (rh3u17@soton.ac.uk / robertoholmes@live.com)
 * @brief Deal with request from main to process serial data.
 *
 * @version 0.1
 * @date 2021-04-01
 *
 * @copyright UOS3
 */

#ifndef APPLICATIONS_SERIAL_COMMS_SERIALCOMMS_PUBLIC_H_
#define APPLICATIONS_SERIAL_COMMS_SERIALCOMMS_PUBLIC_H_

/* -------------------------------------------------------------------------
 * INCLUDES
 * ------------------------------------------------------------------------- */

/* Standard Library */
#include <stdint.h>

/* Internal */
#include "components/serial/Serial_public.h"

#include "applications/config_file/ConfigFile_public.h"
#include "applications/rail_editor/RailEditor_public.h"
#include "applications/house_keeping/HouseKeeping_public.h"
#include "applications/battery_comms/BatteryComms_public.h"

/* -------------------------------------------------------------------------
 * DEFINES
 * ------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------
 * GLOBALS
 * ------------------------------------------------------------------------- */

uint8_t SERIAL_COMMS_PACKET[SERIAL_TX_PACKET_TOTAL_LENGTH];

/* -------------------------------------------------------------------------
 * STRUCTS
 * ------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------
 * FUNCTIONS
 * ------------------------------------------------------------------------- */
int SerialComms_process();
void SerialComms_add_header(uint8_t *p_packet_out, uint8_t response_type);

#endif /* APPLICATIONS_SERIAL_COMMS_SERIALCOMMS_PUBLIC_H_ */
