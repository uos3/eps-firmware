/*
 *
 * @file Serial_public.h
 * @author Roberto Holmes (rh3u17@soton.ac.uk / robertoholmes@live.com)
 * @brief Provide first level of processing to UART packets including
 * dealing with CRC.
 *
 * @version 0.1
 * @date 2021-04-08
 *
 * @copyright UOS3
 */

#ifndef COMPONENTS_SERIAL_SERIAL_PUBLIC_H_
#define COMPONENTS_SERIAL_SERIAL_PUBLIC_H_

/* -------------------------------------------------------------------------
 * INCLUDES
 * ------------------------------------------------------------------------- */

/* Standard Library */
#include <stdint.h>

/* Internal */
#include "drivers/uart/Uart_public.h"

/* -------------------------------------------------------------------------
 * DEFINES
 * ------------------------------------------------------------------------- */

#define SERIAL_PACKET_MAX_LENGTH (10)

/* -------------------------------------------------------------------------
 * GLOBALS
 * ------------------------------------------------------------------------- */

uint8_t SERIAL_PACKET[SERIAL_PACKET_MAX_LENGTH];

/* -------------------------------------------------------------------------
 * STRUCTS
 * ------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------
 * FUNCTIONS
 * ------------------------------------------------------------------------- */

uint8_t Serial_TX(uint8_t *p_packet_in, uint8_t packet_length_in);

uint8_t* Serial_RX(uint8_t *p_valid_packet_out, uint8_t *p_length_out);

#endif /* COMPONENTS_SERIAL_SERIAL_PUBLIC_H_ */
