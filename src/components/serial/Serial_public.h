/*
 * Panels_public.h
 *
 *  Created on: 15 Mar 2021
 *      Author: Roberto
 */

#ifndef COMPONENTS_SERIAL_SERIAL_PUBLIC_H_
#define COMPONENTS_SERIAL_SERIAL_PUBLIC_H_

/* -------------------------------------------------------------------------
 * INCLUDES
 * ------------------------------------------------------------------------- */

/* Standard Library */
#include <stdint.h>

/* Internal */

/* -------------------------------------------------------------------------
 * DEFINES
 * ------------------------------------------------------------------------- */

#define SERIAL_PACKET_MAX_LENGTH (8)

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

int Serial_TX(uint8_t* p_packet_in, uint8_t packet_length_in);

uint8_t* Serial_RX(uint8_t *p_length_out, uint8_t* p_valid_packet_out);


#endif /* COMPONENTS_SERIAL_SERIAL_PUBLIC_H_ */
