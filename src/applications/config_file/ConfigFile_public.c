/*
 *
 * @file ConfigFile_public.c
 * @author Roberto Holmes (rh3u17@soton.ac.uk / robertoholmes@live.com)
 * @brief Public Config File application source file
 *
 * Pass on commands to the flash editor and tell it to consider
 * the config portion of the flash.
 *
 * Task ref: [EPS_SW_1.5.6]
 *
 * @version 0.1
 * @date 2021-04-06
 *
 * @copyright UOS3
 */

#include "ConfigFile_public.h"

/* -------------------------------------------------------------------------
 * GLOBALS
 * ------------------------------------------------------------------------- */

uint8_t CONFIG_FILE_DATA_8BIT[2];

/* -------------------------------------------------------------------------
 * FUNCTIONS
 * ------------------------------------------------------------------------- */

uint8_t ConfigFile_read_8bit(uint8_t address, uint8_t *p_data_out) {
    /* Modify the address so that the flash editor knows it is
     * referring to the config.
     */
    address |= FLASH_EDITOR_CONFIG_BIT;
    return FlashEditor_read(address, p_data_out, 1);
}

uint8_t ConfigFile_read_16bit(uint8_t address, uint16_t *p_data_out) {
    /* Modify the address so that the flash editor knows it is
     * referring to the config.
     */
    address |= FLASH_EDITOR_CONFIG_BIT;

    uint8_t return_value = FlashEditor_read(address, CONFIG_FILE_DATA_8BIT, 2);

    *p_data_out = ((uint16_t) CONFIG_FILE_DATA_8BIT[0])
            + (((uint16_t) CONFIG_FILE_DATA_8BIT[1]) << 8);

    return return_value;
}

uint8_t ConfigFile_write(uint8_t *p_data_in) {
    /* Tell the flash editor to write the data to the config
     * section of the flash. */
    return FlashEditor_write(FLASH_EDITOR_CONFIG_BIT, p_data_in,
                             CONFIG_FILE_LENGTH);
}

