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

#define SERIAL_COMMS_HEADER_LENGTH (4)
#define SERIAL_COMMS_COMMAND_ADDRESS (2)

#define SERIAL_COMMS_COMMAND_UPDATE_CONFIG (0x00)
#define SERIAL_COMMS_COMMAND_SET_RAIL (0x01)
#define SERIAL_COMMS_COMMAND_RESET_RAIL (0x02)
#define SERIAL_COMMS_COMMAND_BATTERY_COMM (0x03)
#define SERIAL_COMMS_COMMAND_HOUSE_KEEPING (0x04)

#define SERIAL_COMMS_RESPONSE_TYPE_ADDRESS (2)

#define SERIAL_COMMS_RESPONSE_UPDATE_CONFIG (0x00)
#define SERIAL_COMMS_RESPONSE_SET_RAIL (0x01)
#define SERIAL_COMMS_RESPONSE_RESET_RAIL (0x02)
#define SERIAL_COMMS_RESPONSE_BATTERY_COMM (0x03)
#define SERIAL_COMMS_RESPONSE_HOUSE_KEEPING (0x04)
#define SERIAL_COMMS_RESPONSE_NO_DATA (0x05)
#define SERIAL_COMMS_RESPONSE_CORRUPTED_DATA (0x06)
#define SERIAL_COMMS_RESPONSE_UNRECOGNISED_COMMAND (0x07)

#define SERIAL_COMMS_PACKET_LENGTH (HOUSE_KEEPING_DATA_LENGTH+SERIAL_COMMS_HEADER_LENGTH)

/* -------------------------------------------------------------------------
 * GLOBALS
 * ------------------------------------------------------------------------- */

uint8_t SERIAL_COMMS_PACKET[SERIAL_COMMS_PACKET_LENGTH];

/* -------------------------------------------------------------------------
 * STRUCTS
 * ------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------
 * FUNCTIONS
 * ------------------------------------------------------------------------- */
int SerialComms_process();
void SerialComms_add_header(uint8_t *p_packet_out, uint8_t response_type);

#endif /* APPLICATIONS_SERIAL_COMMS_SERIALCOMMS_PUBLIC_H_ */
