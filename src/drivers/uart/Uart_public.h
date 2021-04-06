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
#include <msp430.h>


/* -------------------------------------------------------------------------
 * DEFINES
 * ------------------------------------------------------------------------- */
/* Max number of tries to read from/send to buffer*/
#define MAX_TRYS (30)
/*Error: TX buffer full (last character has not been sent)*/
#define UART_TX_BUFFER_FULL_MAX_ATTEMPTS_REACHED (0x01)

/*Error: RX buffer empty (has not received a character)*/
#define UART_RX_BUFFER_EMPTY_MAX_ATTEMPTS_REACHED (0x02)

/* -------------------------------------------------------------------------
 * FUNCTIONS
 * ------------------------------------------------------------------------- */

/* @brief Initialises the UART
 * Follows init procedure recommended pg413 http://www.ti.com/lit/ug/slau144j/slau144j.pdf
 * @return 0 if successful, non-zero if not
 */
void Uart_init(void);
uint8_t Uart_send_bytes(uint8_t *p_buffer_in, uint8_t length_in);
uint8_t Uart_recv_bytes(uint8_t * p_buffer_out, uint8_t length_in);




#endif /* DRIVERS_UART_UART_PUBLIC_H_ */
