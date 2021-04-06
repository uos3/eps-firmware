/*
 *
 * @file ConfigFile_public.c
 * @author Roberto Holmes (rh3u17@soton.ac.uk / robertoholmes@live.com)
 * @brief Pass on commands to the flash editor and tell it to consider
 * the config portion of the flash.
 *
 * @version 0.1
 * @date 2021-04-06
 *
 * @copyright UOS3
 */

#include "ConfigFile_public.h"

uint8_t ConfigFile_read(uint8_t address, uint8_t *p_data_out) {
    /* Modify the address so that the flash editor knows it is
     * referring to the config.
     */
    address |= FLASH_EDITOR_CONFIG_BIT;
    return FlashEditor_read(address, p_data_out);
}

uint8_t ConfigFile_write(uint8_t *p_data_in) {
    /* Tell the flash editor to write the data to the config
     * section of the flash.
     */
    return FlashEditor_write(FLASH_EDITOR_CONFIG_BIT, p_data_in);
}

