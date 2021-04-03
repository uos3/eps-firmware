/*
 *
 * @file HouseKeeping_public.h
 * @author Roberto Holmes (rh3u17@soton.ac.uk / robertoholmes@live.com)
 * @brief Get all the data available to the EPS so that it can be
 * transmitted to the TOBC.
 *
 * @version 0.1
 * @date 2021-04-03
 *
 * @copyright UOS3
 */

#ifndef APPLICATIONS_HOUSE_KEEPING_HOUSEKEEPING_PUBLIC_H_
#define APPLICATIONS_HOUSE_KEEPING_HOUSEKEEPING_PUBLIC_H_

/* -------------------------------------------------------------------------
 * INCLUDES
 * ------------------------------------------------------------------------- */

/* Standard Library */
#include <stdint.h>

/* Internal */
#include "applications/battery_comms/BatteryComms_public.h"
#include "applications/log_file/LogFile_public.h"

#include "components/rails/Rails_public.h"

/* -------------------------------------------------------------------------
 * DEFINES
 * ------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------
 * GLOBALS
 * ------------------------------------------------------------------------- */

#define HOUSE_KEEPING_DATA_LENGTH (110)

/* -------------------------------------------------------------------------
 * STRUCTS
 * ------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------
 * FUNCTIONS
 * ------------------------------------------------------------------------- */

int HouseKeeping_get_data(uint8_t* p_packet_out);

#endif /* APPLICATIONS_HOUSE_KEEPING_HOUSEKEEPING_PUBLIC_H_ */
