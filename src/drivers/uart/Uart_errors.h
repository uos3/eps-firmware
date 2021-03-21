/*
 * @ingroup uart
 *
 * @file Uart_errors.h
 * @author Daniel Murphy dm4g16@soton.ac.uk
 * @brief Error header file for the uart
 *
 * @version 0.1
 * @date 2021-03-21
 *
 * @copyright UOS3
 */

#ifndef DRIVERS_UART_UART_ERRORS_H_
#define DRIVERS_UART_UART_ERRORS_H_

/* -------------------------------------------------------------------------
 * INCLUDES
 * ------------------------------------------------------------------------- */

/* Standard library includes */
#include <stdio.h>


#include "system/kernel/Kernel_errors.h"
/* -------------------------------------------------------------------------
 * DEFINES
 * ------------------------------------------------------------------------- */

/* @brief Circular buffer cannot push data from RX buffer to circular buffer as queue is full*/
#define UART_RX_ERROR_QUEUE_FULL ((ErrorCode) 0x01)

/* @brief Circular buffer which contains RX buffer data is empty, no data to retrieve */
#define UART_RX_ERROR_QUEUE_EMPTY ((ErrorCode) 0x02)

/* @brief Reached maximum number of attempts trying to send data over TX */
#define UART_TX_ERROR_MAX_TRYS_REACHED  ((ErrorCode) 0x03)


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

#endif /* DRIVERS_UART_UART_ERRORS_H_ */
