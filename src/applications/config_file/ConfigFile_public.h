/*
 *
 * @file ConfigFile_public.h
 * @author Roberto Holmes (rh3u17@soton.ac.uk / robertoholmes@live.com)
 * @brief Pass on commands to the flash editor and tell it to consider
 * the config portion of the flash.
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

/* -------------------------------------------------------------------------
 * GLOBALS
 * ------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------
 * DEFINES
 * ------------------------------------------------------------------------- */

#define CONFIG_FILE_LENGTH (6)

/* Address for each config variable */
#define CONFIG_FILE_OCP (0x00)
#define CONFIG_FILE_WDT_INTERVAL (0x01)
#define CONFIG_FILE_WAKE_TIMER (0x02)
#define CONFIG_FILE_TOBC_TIMER (0x04)


/* -------------------------------------------------------------------------
 * STRUCTS
 * ------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------
 * FUNCTIONS
 * ------------------------------------------------------------------------- */
uint8_t ConfigFile_read(uint8_t address, uint8_t *p_data_out);
uint8_t ConfigFile_write(uint8_t *p_data_in);


#endif /* APPLICATIONS_CONFIG_FILE_CONFIGFILE_PUBLIC_H_ */
