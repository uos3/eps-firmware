/*
 *
 * @file LogFile_public.c
 * @author Roberto Holmes (rh3u17@soton.ac.uk / robertoholmes@live.com)
 * @brief Public Log File application source file
 *
 * Implement a cache system for writing log events and pass on
 * log specific commands to the Flash Editor.
 *
 * Task ref: [EPS_SW_1.5.7]
 *
 * @version 0.1
 * @date 2021-04-07
 *
 * @copyright UOS3
 */

#include "LogFile_public.h"

/* Set all the values in the cache to 0 */
void LogFile_init() {
    memset(LOG_FILE_CACHE, 0, LOG_FILE_LENGTH);
}

/* Increments value at address by one  and then the value is returned */
uint8_t LogFile_write(uint8_t address_in) {
    return ++LOG_FILE_CACHE[address_in];
}

/* Output all the values in the cache and the flash through the given pointer */
void LogFile_read(uint8_t *p_packet_out) {
    uint8_t i;
    FlashEditor_write(FLASH_EDITOR_LOG_BIT, p_packet_out);
    for (i = LOG_FILE_LENGTH; i != 0; i--) {
        p_packet_out[i - 1u] += LOG_FILE_CACHE[i - 1u];
    }
}

/* Tell the flash editor to write to the cache to the log portion */
void LogFile_commit() {
    FlashEditor_write(FLASH_EDITOR_LOG_BIT, LOG_FILE_CACHE);

    /* Clear cache */
    memset(LOG_FILE_CACHE, 0, LOG_FILE_LENGTH);
}

/* Tell the flash editor to write 0 to the log portion of flash */
uint8_t LogFile_clear() {
    return FlashEditor_write(FLASH_EDITOR_LOG_BIT, 0);
}

/* Return 1 if there have been any additions to the cache and 0 if not */
uint8_t LogFile_check_changes() {
    uint8_t i;
    for (i = LOG_FILE_LENGTH; i != 0; i--) {
        if (LOG_FILE_CACHE[i - 1u] != 0) {
            return 1;
        }
    }
    return 0;
}
