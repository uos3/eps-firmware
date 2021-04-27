/*
 *
 * @file FlashEditor_public.h
 * @author Roberto Holmes (rh3u17@soton.ac.uk / robertoholmes@live.com)
 * @brief Public Flash Editor component header file
 *
 * Task ref: [EPS_SW_1.6.5]
 *
 * @version 0.1
 * @date 2021-04-12
 *
 * @copyright UOS3
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
#include "applications/log_file/LogFile_public.h"
#include "components/serial/Serial_public.h"
#include "drivers/flash/Flash_public.h"

/* -------------------------------------------------------------------------
 * GLOBALS
 * ------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------
 * DEFINES
 * ------------------------------------------------------------------------- */

/* In the MSB of the address, store whether referring to the config or the log */
#define FLASH_EDITOR_CONFIG_BIT BIT7
#define FLASH_EDITOR_LOG_BIT (0)

/* Place Config in Segment C */
#define FLASH_EDITOR_CONFIG_ADDRESS (0x40)
/* Place Log File in Segment D */
#define FLASH_EDITOR_LOG_ADDRESS (0x00)

/* -------------------------------------------------------------------------
 * STRUCTS
 * ------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------
 * FUNCTIONS
 * ------------------------------------------------------------------------- */

uint8_t FlashEditor_read(uint8_t address_in, uint8_t *p_data_out,
                         uint8_t length_in);
uint8_t FlashEditor_write(uint8_t address_in, uint8_t *p_data_in,
                          uint8_t length_in);

#endif /* COMPONENTS_FLASH_EDITOR_FLASHEDITOR_PUBLIC_H_ */
