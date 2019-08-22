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

// Based on factory set address of battery specified in pg 48 of batt report C
#define BAT_ADDRESS     0x2A

// Based on names of commands in pg 55 of battery report C
#define BAT_I2C_IBAT        0xE28A//0xE284
#define BAT_I2C_TEMPBRD     0xE308


typedef enum {      //Battery commands
    GETERROR = 0x03,
    GETTELEM = 0x10,
    BOARDSTATUS = 0x00,
    GETCHECKSUM = 0x05,
    GETBROWNOUTNO = 0x31,
    GETNOAUTORESETS = 0x32,
    GETNOMANUALRESETS = 0x33,
    MANUALRESET = 0x80
} BATCOMMANDS;


void reset_battery();

uint16_t get_battery_telemetry(uint16_t reg, uint8_t* i2c_buff);

#endif /* HAL_BATTERY_H_ */
