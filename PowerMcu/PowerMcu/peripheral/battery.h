/*
 * hal.h
 *
 *  Created on: 05 Sept 2018
 *      Author: Tom Piga
 */

#ifndef HAL_BATTERY_H_
#define HAL_BATTERY_H_

#include <msp430.h>
#include <stdint.h>

#define BAT_ADDRESS     0x2A

#define BAT_I2C_IBAT        0xE284
#define BAT_I2C_TEMPBRD     0xE308

#define BAT_WRITE_CMD   1
#define BAT_READ_CMD    0

uint16_t get_battery_telemetry(uint16_t reg, uint8_t* i2c_buff);

#endif /* HAL_BATTERY_H_ */
