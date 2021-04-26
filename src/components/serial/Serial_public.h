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
#include "applications/house_keeping/HouseKeeping_public.h"
#include "drivers/uart/Uart_public.h"
#include "util/crc/Crc_public.h"

/* -------------------------------------------------------------------------
 * DEFINES
 * ------------------------------------------------------------------------- */

/* Frame number for sending a packet that is unsolicited by the TOBC */
#define SERIAL_UNSOLICITED_FRAME_NUM (0)

/* Possible commands received from TOBC */
/* Get all house keeping data */
#define SERIAL_COMMAND_HOUSE_KEEPING (1)
/* Change the values in the config to the ones in the data packet */
#define SERIAL_COMMAND_UPDATE_CONFIG (2)
/* Set the state of the OCP rails */
#define SERIAL_COMMAND_SET_RAIL (3)
/* Pass on a command to the battery */
#define SERIAL_COMMAND_BATTERY_COMM (4)
/* Set an OCP rail to OFF then ON */
#define SERIAL_COMMAND_RESET_RAIL (5)

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
/* Data type is not recognised */
#define SERIAL_PAYLOAD_SIZE_UNRECOGNISED_COMMAND (1)
/* Header length is not 2 */
#define SERIAL_PAYLOAD_SIZE_INVALID_HEADER (0)
/* Packet length is 0 */
#define SERIAL_PAYLOAD_SIZE_INVALID_LENGTH (0)
/* Failed CRC on RX packet */
#define SERIAL_PAYLOAD_SIZE_CORRUPTED_DATA (0)
/* Failed CRC on flash read */
#define SERIAL_PAYLOAD_SIZE_FLASH_READ_FAIL (0)

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

/* Data type is not recognised */
#define SERIAL_RESPONSE_UNRECOGNISED_COMMAND (134)
/* Header length is not 2 */
#define SERIAL_RESPONSE_INVALID_HEADER (135)
/* */
#define SERIAL_RESPONSE_INVALID_LENGTH (136)
/* Failed CRC on RX packet */
#define SERIAL_RESPONSE_CORRUPTED_DATA (137)
/* Failed CRC on flash read */
#define SERIAL_RESPONSE_FLASH_READ_FAIL (138)

/* Define TX packet lengths */
#define SERIAL_HEADER_LENGTH (2)
#define SERIAL_TX_PACKET_TOTAL_LENGTH (SERIAL_HEADER_LENGTH + HOUSE_KEEPING_DATA_LENGTH + CRC_LENGTH)

/* Length of RX Header + Payload + CRC */
#define SERIAL_RX_PACKET_MAX_LENGTH (7)

/* -------------------------------------------------------------------------
 * GLobals
 * ------------------------------------------------------------------------- */

uint8_t SERIAL_TX_PACKET[SERIAL_HEADER_LENGTH + 1 + CRC_LENGTH];

/* -------------------------------------------------------------------------
 * FUNCTIONS
 * ------------------------------------------------------------------------- */

uint8_t Serial_TX(uint8_t *p_packet_in, uint8_t response_type_in,
                  uint8_t frame_number_in, uint8_t packet_length_in);
uint8_t Serial_read_RX(uint8_t *p_frame_number_out,
                        uint8_t *p_valid_packet_out, uint8_t *p_length_out, uint8_t *p_data_out);
uint8_t Serial_process_RX();

#endif /* COMPONENTS_SERIAL_SERIAL_PUBLIC_H_ */
