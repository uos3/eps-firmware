/*
 *
 * @file ConfigFile_public.h
 * @author Roberto Holmes (rh3u17@soton.ac.uk / robertoholmes@live.com)
 * @brief Public Config File application header file
 *
 * Task ref: [EPS_SW_1.5.6]
 *
 * @version 0.1
 * @date 2021-04-06
 *
 * @copyright UOS3
 */

#ifndef APPLICATIONS_CONFIG_FILE_CONFIGFILE_PUBLIC_H_
#define APPLICATIONS_CONFIG_FILE_CONFIGFILE_PUBLIC_H_

/* -------------------------------------------------------------------------
 * INCLUDES
 * ------------------------------------------------------------------------- */

/* Standard Library */
#include <stdint.h>

/* Internal */
#include "components/flash_editor/FlashEditor_public.h"
#include "util/crc/Crc_public.h"

/* -------------------------------------------------------------------------
 * DEFINES
 * ------------------------------------------------------------------------- */

#define CONFIG_FILE_LENGTH (3)

/* Address for each config variable */
/* Which rails should remain on after an OCP event */
#define CONFIG_FILE_RESET_RAIL_AFTER_OCP (0x00)
/* How long the timer should count for before resetting the TOBC
 * (divide by 2048 to get value in s)*/
#define CONFIG_FILE_TOBC_TIMER (0x01)

/* -------------------------------------------------------------------------
 * FUNCTIONS
 * ------------------------------------------------------------------------- */
uint8_t ConfigFile_read_8bit(uint8_t address, uint8_t *p_data_out);
uint8_t ConfigFile_read_16bit(uint8_t address, uint16_t *p_data_out);
void ConfigFile_write(uint8_t *p_data_in);


#endif /* APPLICATIONS_CONFIG_FILE_CONFIGFILE_PUBLIC_H_ */
