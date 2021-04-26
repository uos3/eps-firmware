/*
 *
 * @file LogFile_public.h
 * @author Roberto Holmes (rh3u17@soton.ac.uk / robertoholmes@live.com)
 * @brief Public Log File application header file
 *
 * Task ref: [EPS_SW_1.5.7]
 *
 * @version 0.1
 * @date 2021-04-07
 *
 * @copyright UOS3
 */


#ifndef APPLICATIONS_LOG_FILE_LOGFILE_PUBLIC_H_
#define APPLICATIONS_LOG_FILE_LOGFILE_PUBLIC_H_

/* -------------------------------------------------------------------------
 * INCLUDES
 * ------------------------------------------------------------------------- */

/* Standard library includes */
#include <msp430.h>
#include <stdint.h>
#include <string.h>

/* Internal includes */
#include "components/flash_editor/FlashEditor_public.h"
#include "util/crc/Crc_public.h"

/* -------------------------------------------------------------------------
 * DEFINES
 * ------------------------------------------------------------------------- */

#define LOG_FILE_LENGTH (8)

/* Position in cache of each log variable:
 * - Bytes 0-5 record how many times each OCP rail has tripped
 * - Byte 6 stores how many system reboots have occurred
 * - Byte 7 is how many times the TOBC timer has elapsed and a reboot has been ordered
 */
#define LOG_FILE_OCP1_ADDR (0)
#define LOG_FILE_OCP2_ADDR (1)
#define LOG_FILE_OCP3_ADDR (2)
#define LOG_FILE_OCP4_ADDR (3)
#define LOG_FILE_OCP5_ADDR (4)
#define LOG_FILE_OCP6_ADDR (5)
#define LOG_FILE_BOOT_COUNT_ADDR (6)
#define LOG_FILE_TOBC_TIMER_COUNT_ADDR (7)

/* -------------------------------------------------------------------------
 * GLOBALS
 * ------------------------------------------------------------------------- */

uint8_t LOG_FILE_CACHE [LOG_FILE_LENGTH+CRC_LENGTH];

/* -------------------------------------------------------------------------
 * STRUCTS
 * ------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------
 * FUNCTIONS
 * ------------------------------------------------------------------------- */

void LogFile_init();
uint8_t LogFile_write(uint8_t address_in);
void LogFile_read(uint8_t* p_packet_out);
void LogFile_commit();
uint8_t LogFile_clear();
uint8_t LogFile_check_changes();


#endif /* APPLICATIONS_LOG_FILE_LOGFILE_PUBLIC_H_ */
