/*
 * LogFile_public.h
 *
 *  Created on: 29 Mar 2021
 *      Author: Roberto
 */

#ifndef APPLICATIONS_LOG_FILE_LOGFILE_PUBLIC_H_
#define APPLICATIONS_LOG_FILE_LOGFILE_PUBLIC_H_

/* -------------------------------------------------------------------------
 * INCLUDES
 * ------------------------------------------------------------------------- */

/* Standard library includes */
#include <msp430.h>
#include <stdint.h>

/* Internal includes */

/* -------------------------------------------------------------------------
 * DEFINES
 * ------------------------------------------------------------------------- */

#define LOG_FILE_LENGTH (8)

/* Position in cache of each log variable */
#define LOG_FILE_OCP1_ADDR (0)
#define LOG_FILE_BOOT_COUNT_ADDR (6)
#define LOG_FILE_TOBC_TIMER_COUNT_ADDR (7)

/* -------------------------------------------------------------------------
 * GLOBALS
 * ------------------------------------------------------------------------- */

uint8_t LOG_FILE_CACHE [LOG_FILE_LENGTH];

/* -------------------------------------------------------------------------
 * STRUCTS
 * ------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------
 * FUNCTIONS
 * ------------------------------------------------------------------------- */

int LogFile_write(uint8_t address_in);
int LogFile_read(uint8_t* p_packet_in);
int LogFile_commit();
int LogFile_clear();
int LogFile_check_changes();


#endif /* APPLICATIONS_LOG_FILE_LOGFILE_PUBLIC_H_ */
