/*
 *
 * @file Serial_public.h
 * @author Roberto Holmes (rh3u17@soton.ac.uk / robertoholmes@live.com)
 * @brief Public Serial component header file
 *
 * Task ref: [EPS_SW_1.6.1]
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
#include "util/crc/Crc_public.h"

/* -------------------------------------------------------------------------
 * DEFINES
 * ------------------------------------------------------------------------- */

/* Frame number for sending a packet that is unsolicited by the TOBC */
#define SERIAL_UNSOLICITED_FRAME_NUM (0)

/* Position in packet for the byte containing the command */
#define SERIAL_COMMAND_ADDRESS (1)

/* Possible commands received from TOBC */
/* Get all house keeping data */
#define SERIAL_COMMAND_HOUSE_KEEPING (0)
/* Change the values in the config to the ones in the data packet */
#define SERIAL_COMMAND_UPDATE_CONFIG (1)
/* Set the state of the OCP rails */
#define SERIAL_COMMAND_SET_RAIL (2)
/* Pass on a command to the battery */
#define SERIAL_COMMAND_BATTERY_COMM (3)
/* Set an OCP rail to OFF then ON */
#define SERIAL_COMMAND_RESET_RAIL (4)

/* Payload size for each command received from TOBC */
/* Get all house keeping data */
#define SERIAL_PAYLOAD_SIZE_HOUSE_KEEPING (0)
/* Change the values in the config to the ones in the data packet */
#define SERIAL_PAYLOAD_SIZE_UPDATE_CONFIG (3)
/* Set the state of the OCP rails */
#define SERIAL_PAYLOAD_SIZE_SET_RAIL (1)
/* Pass on a command to the battery */
#define SERIAL_PAYLOAD_SIZE_BATTERY_COMM (3)
/* Set an OCP rail to OFF then ON */
#define SERIAL_PAYLOAD_SIZE_RESET_RAIL (1)

/* Possible responses EPS can send to the TOBC */
/* Return house keeping data */
#define SERIAL_RESPONSE_HOUSE_KEEPING (129)
/* Return the updated config */
#define SERIAL_RESPONSE_UPDATE_CONFIG (130)
/* Return current OCP rail state */
#define SERIAL_RESPONSE_SET_RAIL (131)
/* Return battery response */
#define SERIAL_RESPONSE_BATTERY_COMM (132)
/* Return battery response */
#define SERIAL_RESPONSE_OCP_EVENT (133)

/* TODO: add to ICD */
//#define SERIAL_RESPONSE_NO_DATA (0x05)
//#define SERIAL_RESPONSE_CORRUPTED_DATA (0x06)
//#define SERIAL_RESPONSE_UNRECOGNISED_COMMAND (0x07)

/* Define TX packet lengths */
#define SERIAL_HEADER_LENGTH (2)
#define SERIAL_PAYLOAD_LENGTH (128)
#define SERIAL_TX_PACKET_TOTAL_LENGTH (SERIAL_HEADER_LENGTH + SERIAL_PAYLOAD_LENGTH + CRC_LENGTH)

/* Length of RX Header + Payload + CRC */
#define SERIAL_RX_PACKET_MAX_LENGTH (7)

/* -------------------------------------------------------------------------
 * FUNCTIONS
 * ------------------------------------------------------------------------- */

uint8_t Serial_TX(uint8_t *p_packet_in, uint8_t response_type_in,
                  uint8_t frame_number_in, uint8_t packet_length_in);
uint8_t* Serial_read_RX(uint8_t *p_frame_number_out,
                        uint8_t *p_valid_packet_out, uint8_t *p_length_out);
uint8_t Serial_process_RX();

#endif /* COMPONENTS_SERIAL_SERIAL_PUBLIC_H_ */
