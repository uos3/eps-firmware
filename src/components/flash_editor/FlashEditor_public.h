/*
 * Config_public.h
 *
 *  Created on: 15 Mar 2021
 *      Author: Roberto
 */

#ifndef COMPONENTS_FLASH_EDITOR_FLASHEDITOR_PUBLIC_H_
#define COMPONENTS_FLASH_EDITOR_FLASHEDITOR_PUBLIC_H_

/* -------------------------------------------------------------------------
 * INCLUDES
 * ------------------------------------------------------------------------- */

/* Standard Library */
#include <stdint.h>
#include <msp430.h>

/* Internal */
#include "applications/config_file/ConfigFile_public.h"

/* -------------------------------------------------------------------------
 * GLOBALS
 * ------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------
 * DEFINES
 * ------------------------------------------------------------------------- */

/* In the MSB of the address, store whether referring to the config or the log */
#define FLASH_EDITOR_CONFIG_BIT BIT7
#define FLASH_EDITOR_LOG_BIT (0x00)

/* -------------------------------------------------------------------------
 * STRUCTS
 * ------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------
 * FUNCTIONS
 * ------------------------------------------------------------------------- */

uint8_t FlashEditor_read(uint8_t address, uint8_t*p_data_out);
uint8_t FlashEditor_write(uint8_t address, uint8_t*p_data_in);

#endif /* COMPONENTS_FLASH_EDITOR_FLASHEDITOR_PUBLIC_H_ */
