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
#include "components/rails/Rails_public.h"

#include "applications/config_file/ConfigFile_public.h"
#include "applications/rail_editor/RailEditor_public.h"
#include "applications/house_keeping/HouseKeeping_public.h"
#include "applications/battery_comms/BatteryComms_public.h"

#include "util/crc/Crc_public.h"
#include "util/convert_16bit/Convert_16bit_public.h"

/* -------------------------------------------------------------------------
 * DEFINES
 * ------------------------------------------------------------------------- */

#define SERIAL_COMMS_VALID_CONTINUE (0)
#define SERIAL_COMMS_INVALID_CONTINUE (1)

/* -------------------------------------------------------------------------
 * GLOBALS
 * ------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------
 * STRUCTS
 * ------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------
 * FUNCTIONS
 * ------------------------------------------------------------------------- */
void SerialComms_continue(uint8_t valid_continue_in);
void SerialComms_prepare_packet();

#endif /* APPLICATIONS_SERIAL_COMMS_SERIALCOMMS_PUBLIC_H_ */
