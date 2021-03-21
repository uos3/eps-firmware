/*
 * @ingroup uart
 *
 * @file Uart_public.h
 * @author Daniel Murphy (dm4g16@soton.ac.uk)
 * @brief Public header for UART driver.
 *
 * @version 0.1
 * @date 2021-03-15
 *
 * @copyright UOS3
 */

#ifndef DRIVERS_UART_UART_PUBLIC_H_
#define DRIVERS_UART_UART_PUBLIC_H_

/* -------------------------------------------------------------------------
 * INCLUDES
 * ------------------------------------------------------------------------- */

/* Standard Library */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* Internal */
#include "util/circular_buffers/Circular_buffer_public.h"
#include <msp430.h>
#include "drivers/uart/Uart_errors.h"

/* -------------------------------------------------------------------------
 * GLOBALS
 * ------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------
 * DEFINES
 * ------------------------------------------------------------------------- */
static circ rxbuffer;
#define MAX_TRYS (30)
/* -------------------------------------------------------------------------
 * STRUCTS
 * ------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------
 * FUNCTIONS
 * ------------------------------------------------------------------------- */

/* @brief Initialises the UART
 * Follows init procedure recommended pg413 http://www.ti.com/lit/ug/slau144j/slau144j.pdf
 * Need to check with duncan and leo about error handling within this softwawre
 * may not need to be as sophisticated as in the obc software
 * @return ErrorCode: if no error ERROR_NONE or UART_ERROR_x
 */
void Uart_init(void);
ErrorCode Uart_send_bytes(uint8_t *p_buffer, uint8_t offset, uint8_t length);
ErrorCode Uart_recv_bytes(uint8_t * p_buffer, uint8_t offset, uint8_t count);




#endif /* DRIVERS_UART_UART_PUBLIC_H_ */
