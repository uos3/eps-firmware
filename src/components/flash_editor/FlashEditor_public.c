/*
 *
 * @file FlashEditor_public.c
 * @author Roberto Holmes (rh3u17@soton.ac.uk / robertoholmes@live.com)
 * @brief Public Flash Editor component source file
 *
 * Task ref: [EPS_SW_1.6.5]
 *
 * @version 0.1
 * @date 2021-04-12
 *
 * @copyright UOS3
 */

#include "FlashEditor_public.h"

uint8_t FlashEditor_read(uint8_t address_in, uint8_t *p_data_out, uint8_t length_in) {
    /* Check if the bit identifying that the address is for the config is set */
    if ((address_in & FLASH_EDITOR_CONFIG_BIT) == FLASH_EDITOR_CONFIG_BIT) {
        /* Remove config identifier bit */
        address_in &= ~FLASH_EDITOR_CONFIG_BIT;
        /* If we are asking for the OCP rail config (only 1 Byte)*/
        if (address_in == 0) {
            Flash_read(FLASH_EDITOR_CONFIG_ADDRESS, 1, p_data_out);
        }
        else {
            /* Rest of data is 2 Bytes */
            address_in += FLASH_EDITOR_CONFIG_ADDRESS;
            Flash_read(address_in, 2, p_data_out);
        }
    }
    /* Else it is a request for log data */
    else {
        Flash_read(FLASH_EDITOR_LOG_ADDRESS, LOG_FILE_LENGTH, p_data_out);
    }
    return 0;
}

uint8_t FlashEditor_write(uint8_t address_in, uint8_t *p_data_in) {
    if (address_in == FLASH_EDITOR_CONFIG_BIT) {
        Flash_write(FLASH_EDITOR_CONFIG_ADDRESS, CONFIG_FILE_LENGTH, p_data_in);
    }
    else {
        Flash_write(FLASH_EDITOR_LOG_ADDRESS, LOG_FILE_LENGTH, p_data_in);
    }
    return 0;
}

