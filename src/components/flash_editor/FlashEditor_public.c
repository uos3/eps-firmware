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

uint8_t FLASH_EDITOR_STORED_DATA[LOG_FILE_LENGTH + CRC_LENGTH];

uint8_t FlashEditor_read(uint8_t address_in, uint8_t *p_data_out,
                         uint8_t length_in) {
    uint8_t i;
    /* Check if the bit identifying that the address is for the config is set */
    if ((address_in & FLASH_EDITOR_CONFIG_BIT) == FLASH_EDITOR_CONFIG_BIT) {
        /* read the stored crc */
        Flash_read(FLASH_EDITOR_CONFIG_ADDRESS, CONFIG_FILE_LENGTH + CRC_LENGTH,
                   FLASH_EDITOR_STORED_DATA);
        /* Check if CRC is passed */
        if (Crc_decode(FLASH_EDITOR_STORED_DATA,
        CONFIG_FILE_LENGTH + 2) == CRC_NO_ERROR_DETECTED) {
            /* Remove config identifier bit */
            address_in &= ~FLASH_EDITOR_CONFIG_BIT;
            /* If we are asking for the ocp_reset byte */
            if (address_in == 0) {
                p_data_out[0] = FLASH_EDITOR_STORED_DATA[0];
            }
            /* If not, return the TOBC timer length */
            else {
                p_data_out[0] = FLASH_EDITOR_STORED_DATA[1];
                p_data_out[1] = FLASH_EDITOR_STORED_DATA[2];
            }
        }
        else {
            /* If CRC has failed, fill the data with 1 and return an error */
            for (i = 0; i < length_in; i++) {
                p_data_out[i] = 0xFF;

            }
            return 1;
        }
    }
    /* Else it is a request for log data */
    else {
        Flash_read(FLASH_EDITOR_LOG_ADDRESS, LOG_FILE_LENGTH + CRC_LENGTH,
                   FLASH_EDITOR_STORED_DATA);
        /* Check if CRC is passed */
        if (Crc_decode(FLASH_EDITOR_STORED_DATA,
        LOG_FILE_LENGTH + 2) == CRC_NO_ERROR_DETECTED) {
            /* If we are asking for the ocp_reset byte */
            p_data_out[0] = FLASH_EDITOR_STORED_DATA[address_in];
        }
        /* If CRC has failed, fill the data with 1 and return an error */
        else {
            for (i = 0; i < length_in; i++) {
                p_data_out[i] = 0xFF;

            }
            return 1;
        }
    }
    return 0;
}

void FlashEditor_write(uint8_t address_in, uint8_t *p_data_in,
                          uint8_t length_in) {
    uint8_t i;
    for (i = 0; i < length_in; i++) {
        FLASH_EDITOR_STORED_DATA[i] = p_data_in[i];
    }
    /* For simulating flash failure */
//    Crc_generate_invalid(FLASH_EDITOR_STORED_DATA, length_in);
    Crc_encode(FLASH_EDITOR_STORED_DATA, length_in);
    if (address_in == FLASH_EDITOR_CONFIG_BIT) {
        Flash_write(FLASH_EDITOR_CONFIG_ADDRESS, length_in + CRC_LENGTH,
                    FLASH_EDITOR_STORED_DATA);
    }
    else {
        Flash_write(FLASH_EDITOR_LOG_ADDRESS, length_in + CRC_LENGTH,
                    FLASH_EDITOR_STORED_DATA);
    }
}

