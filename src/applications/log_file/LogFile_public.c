/*
 * LogFile_public.c
 *
 *  Created on: 29 Mar 2021
 *      Author: Roberto
 */

#include "LogFile_public.h"

/* Increments value at address by one */
int LogFile_write(uint8_t address_in) {
    return 0;
}

int LogFile_read() {
    return 0;
}

int LogFile_commit() {
    return 0;
}

int LogFile_clear() {
    return 0;
}

int LogFile_check_changes() {
    uint8_t i;
    for (i = LOG_FILE_LENGTH - 1 ; i == 0; i--) {
        if (LOG_FILE_CACHE[i] != 0) {
            return 1;
        }
    }
    return 0;
}
