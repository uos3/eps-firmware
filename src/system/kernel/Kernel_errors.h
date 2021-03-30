/*
@ingroup kernel
@file Kernel_errors.h
@author Daniel Murphy (dm4g16@soton.ac.uk)
@brief Definition of standard event error code

@version 0.1
@date 2021-03-21

@copyright UOS3
 */

#ifndef SYSTEM_KERNEL_KERNEL_ERRORS_H_
#define SYSTEM_KERNEL_KERNEL_ERRORS_H_

/* -------------------------------------------------------------------------
 * INCLUDES
 * ------------------------------------------------------------------------- */

/* Standard library includes */
#include <stdio.h>
/* -------------------------------------------------------------------------
 * TYPES
 * ------------------------------------------------------------------------- */
typedef uint16_t ErrorCode;

/* -------------------------------------------------------------------------
 * DEFINES
 * ------------------------------------------------------------------------- */
#define ERROR_NONE ((ErrorCode)0x00)
/* -------------------------------------------------------------------------
 * GLOBALS
 * ------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------
 * ENUMS
 * ------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------
 * STRUCTS
 * ------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------
 * FUNCTIONS
 * ------------------------------------------------------------------------- */

#endif /* SYSTEM_KERNEL_KERNEL_ERRORS_H_ */