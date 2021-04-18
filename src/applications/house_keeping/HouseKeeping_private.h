/*
 *
 * @file HouseKeeping_private.h
 * @author Roberto Holmes (rh3u17@soton.ac.uk / robertoholmes@live.com)
 * @brief Private House Keeping header file
 *
 * Task ref: [EPS_SW_1.5.3]
 *
 * @version 0.1
 * @date 2021-04-09
 *
 * @copyright UOS3
 */

#ifndef APPLICATIONS_HOUSE_KEEPING_HOUSEKEEPING_PRIVATE_H_
#define APPLICATIONS_HOUSE_KEEPING_HOUSEKEEPING_PRIVATE_H_

/* -------------------------------------------------------------------------
 * INCLUDES
 * ------------------------------------------------------------------------- */

/* Standard Library */
#include <stdint.h>

/* Internal */
#include "applications/battery_comms/BatteryComms_public.h"
#include "components/rails/Rails_public.h"
#include "util/convert_16bit/Convert_16bit_public.h"

/* -------------------------------------------------------------------------
 * DEFINES
 * ------------------------------------------------------------------------- */

#define HOUSE_KEEPING_TLM_VBAT (0xE280)
#define HOUSE_KEEPING_TLM_IBAT (0xE284)
#define HOUSE_KEEPING_TLM_IDIRBAT (0xE28E)

#define HOUSE_KEEPING_TLM_TBRD (0xE308)

#define HOUSE_KEEPING_TLM_IPCM5V (0xE214)
#define HOUSE_KEEPING_TLM_VPCM5V (0xE210)
#define HOUSE_KEEPING_TLM_IPCM3V3 (0xE204)
#define HOUSE_KEEPING_TLM_VPCM3V3 (0xE200)

#define HOUSE_KEEPING_TLM_TBAT1 (0xE398)
#define HOUSE_KEEPING_TLM_HBAT1 (0xE39F)
#define HOUSE_KEEPING_TLM_TBAT2 (0xE3A8)
#define HOUSE_KEEPING_TLM_HBAT2 (0xE3AF)
#define HOUSE_KEEPING_TLM_TBAT3 (0xE3B8)
#define HOUSE_KEEPING_TLM_HBAT3 (0xE3BF)

/* -------------------------------------------------------------------------
 * FUNCTIONS
 * ------------------------------------------------------------------------- */

void HouseKeeping_get_bat_data(uint8_t* p_packet_out);

#endif /* APPLICATIONS_HOUSE_KEEPING_HOUSEKEEPING_PRIVATE_H_ */
