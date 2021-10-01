/*
 *
 * @file Battery_public.h
 * @author Roberto Holmes (rh3u17@soton.ac.uk / robertoholmes@live.com)
 * @brief Public Battery component header file
 *
 * Task ref: [EPS_SW_1.6.2]
 *
 * @version 0.1
 * @date 2021-04-09
 *
 * @copyright UOS3
 */

#ifndef COMPONENTS_BATTERY_BATTERY_PUBLIC_H_
#define COMPONENTS_BATTERY_BATTERY_PUBLIC_H_

/* -------------------------------------------------------------------------
 * INCLUDES
 * ------------------------------------------------------------------------- */

/* Standard Library */
#include <stdint.h>

/* Internal */
#include "drivers/i2c/I2c_public.h"

/* -------------------------------------------------------------------------
 * DEFINES
 * ------------------------------------------------------------------------- */

#define BATTERY_I2C_ADDR (0x2A)

/**
 * @brief command to request board status from battery. Note that only Data byte 0 (last byte) is returned (byte 1 is unused).
 * Sending the command 0x01 must be followed by a data byte 0x00
 * 
 */
#define BATTERY_BOARD_STAT_CMD (0x01)

/**
 * @brief If an error has been generated value 0xFFFF will be returned by the battery. By sending cmd 0x03 followed by data byte 
 * 0x00 the error code of the last error detected will be returned. List of error codes can be found in Clyde Space User Manual
 * table 12-3
 * 
 */
#define BATTERY_GET_LAST_ERR_CMD (0x03)

/**
 * @brief returns firware version running on the battery module. 2 bytes are returned; from bit 15(MSB) to bit 12 (nibble 3) corresponds to 
 * battery firmware revision and nibbles 2, 1 and 0 corresponds to battery firmware number.
 * 
 * If 0xFA and 0x23 are the 2 bytes returned, 0xF (nibble 3) is the battery FW revision, while 0xA23 is the battery firmware number 
 * 
 */
#define BATTERY_GET_VERSION_CMD (0x04)

/**
 * @brief value retrieved is used to determine whether content in ROM have changed during operation of device
 * 
 */
#define BATTERY_GET_CHECKSUM_CMD (0x05)

/**
 * @brief board will reset within 1s and brought back to initial conditions of the battery
 * Note that there will be an increment in the Manual Reset Counter
 * 
 */
#define BATTERY_MANUAL_RESET_CMD (0x80)

/**
 * @brief Allows EPS MCU to monitor the operation of the battery. the battery MCU will connect the desired telemetry channel to the ADC
 * To receive the request via I2C, the cmd 0x10 followed by 2 bytes of the data requested need to be sent.
 * 2 bytes will be returned but only the last 10 bits have the ADC result. 
 * 
 * A delay of 5ms is necessary from the TX operation to RX.
 * Note: if TLM requested is not available, channel error will be generated.
 * 
 */
#define BATTERY_GET_TLM_CMD (0x10)

/* -------------------------------------------------------------------------
 * TELEMETRY DATA TYPES
 * ------------------------------------------------------------------------- */

/**
 * @brief Output Battery Voltage (V)
 * Multiply the ADC value by 0.008993
 * 
 */
#define BATTERY_TLM_BATT_DATA       (0xE280)

/**
 * @brief Output Battery Current (mA)
 * Multiply ADC value by 14.662757 
 * 
 */
#define BATTERY_TLM_IBATT_DATA      (0xE284)

/**
 * @brief Current direction
 * If ADC value <512 the battery is charging
 * 
 */
#define BATTERY_TLM_IDIR_BATT_DATA  (0xE28E)

/**
 * @brief Motherboard temperature (*C)
 * Temperature found by multimplying the ADC value
 * by 0.372434 and substracting 273.15
 * 
 */
#define BATTERY_TLM_TBRD_DATA       (0xE308)

/**
 * @brief 5V bus current draw (mA)
 * Multiply the ADC value by 1.327547 
 * 
 */
#define BATTERY_TLM_IPCM5V_DATA     (0xE214)

/**
 * @brief 5V rail output voltage (V)
 * Multiply ADC value by 0.005865
 * 
 */
#define BATTERY_TLM_VPCM5V_DATA     (0xE210)

/**
 * @brief 3V3 bus current draw (mA)
 * Multiply the ADC value by 1.327547 
 * 
 */
#define BATTERY_TLM_IPCM3V3_DATA    (0xE204)

/**
 * @brief 3V3 rail output voltage (V)
 * Multiply ADC value by 0.004311
 * 
 */
#define BATTERY_TLM_VPCM3V3_DATA    (0xE200)

/**
 * @brief Daughterboard temperature (*C)
 * Multiply ADC value by 0.397600 and substract 238.57
 * 
 */
#define BATTERY_TLM_TBATT1_DATA     (0xE398)

/**
 * @brief Daughterboard heater status
 * If ADC value < 512 heater is off, if ADC > 512 heater is on
 * 
 */
#define BATTERY_TLM_HBATT1_DATA     (0xE39F) 

/**
 * @brief Returns the number of brown-out resets on the battery 
 * Note: counter rolls over to 0 after 255 (8 bits max value in decimal)
 * 
 */
#define BATTERY_GET_NUMBER_BROWN_OUT_RESETS_CMD (0x31)

/**
 * @brief Returns number of automatic Software Resets 
 * If on-board MCU on the battery has experienced a malfunction, it automatically resets
 * 
 */
#define BATTERY_GET_NUMBER_AUTIMATIC_SW_RESET_CMD (0x32)

/**
 * @brief Returns number of manual resets that have been done via the reset command
 * 
 */
#define BATTERY_GET_NUMBER_MANUAL_RESET (0x33)

/**
 * @brief Returns heater controller status
 * If 0x00 thermostat control circuitry is DISABLED (heater will remain off, regardless of condition)
 * If 0x01 thermostat control circuitry ENABLED (heater will switch on-board thermostat senses if its appropiate)
 * 
 */
#define BATTERY_GET_HEATER_CONTROLLER_STAT_CMD (0x90)

/**
 * @brief Command that allows to set the heater controller status
 * 
 */
#define BATTERY_SET_HEATER_CONTROLLER_STAT_CMD (0x91)


#define BATTERY_PACKET_MAX_LENGTH (3)
#define BATTERY_RX_PACKET_LENGTH (2)

/* -------------------------------------------------------------------------
 * GLOBALS
 * ------------------------------------------------------------------------- */

uint8_t BATTERY_PACKET[BATTERY_PACKET_MAX_LENGTH];

/* -------------------------------------------------------------------------
 * STRUCTS
 * ------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------
 * FUNCTIONS
 * ------------------------------------------------------------------------- */

uint8_t Battery_TX(uint8_t command_in, uint16_t data_in);

uint16_t Battery_RX();


#endif /* COMPONENTS_BATTERY_BATTERY_PUBLIC_H_ */
