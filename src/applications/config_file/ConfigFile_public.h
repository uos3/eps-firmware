/*
 * ConfigUpdate_public.h
 *
 *  Created on: 15 Mar 2021
 *      Author: Roberto
 */

#ifndef APPLICATIONS_CONFIG_FILE_CONFIGFILE_PUBLIC_H_
#define APPLICATIONS_CONFIG_FILE_CONFIGFILE_PUBLIC_H_

/* -------------------------------------------------------------------------
 * INCLUDES
 * ------------------------------------------------------------------------- */

/* Standard Library */
#include <stdint.h>

/* Internal */


/* -------------------------------------------------------------------------
 * GLOBALS
 * ------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------
 * DEFINES
 * ------------------------------------------------------------------------- */

/* Address for each config variable */
#define CONFIG_FILE_OCP (0x00)


/* -------------------------------------------------------------------------
 * STRUCTS
 * ------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------
 * FUNCTIONS
 * ------------------------------------------------------------------------- */
uint8_t ConfigFile_read(uint8_t address);

int ConfigFile_write(uint8_t address, uint8_t data);


#endif /* APPLICATIONS_CONFIG_FILE_CONFIGFILE_PUBLIC_H_ */
