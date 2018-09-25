/*
 * battery.c
 *
 *  Created on: 5 Sept 2018
 *      Author: Tom Piga
 *      Note: This has been revised after experimenting with I2C so there's
 *      a good chance it will work. However it should be tested with the
 *      engineering model battery, most likely it will only need small changes
 *      (since I don't have the hardware myself to test-and-fix easy errors).
 */

#include "battery.h"

// Conversion from 10-bit battery ADC to real values (pg 55 of battery report C)
uint16_t apply_equation_to_adc(uint16_t value, uint16_t reg){
    float float_value = (float)value;

    switch (reg){
    case BAT_I2C_IBAT:
        float_value *= 14.662757;
        break;
    case BAT_I2C_TEMPBRD:
        float_value *= 0.372434;
        break;
    }

    return (uint16_t) float_value;
}

uint8_t BATTERY_WRITE_CMD_SIZE = 3;
uint8_t BATTERY_READ_CMD_SIZE = 2;
uint16_t get_battery_telemetry(uint16_t reg, uint8_t* i2c_buff){
    // buffer for command to battery
    uint8_t data_to_send[BATTERY_WRITE_CMD_SIZE];

    // [command] part (pg 49 of battery report C)
    data_to_send[0] = 0x10;

    // 16-bit [data parameter] part (pg 55 of battery report C)
    data_to_send[1] = (uint8_t)(0x00FF & reg);
    data_to_send[2] = (uint8_t)(reg >> 8);

    // send command message to battery
    i2c_masterWrite(BAT_ADDRESS, BATTERY_WRITE_CMD_SIZE, &data_to_send);

    // wait until command is sent
    while (!i2c_getTxDoneFlag()){
      for (uint8_t i=0; i<100; i++); //delay
    }
    i2c_clearTxDoneFlag();

    // make buffer for data
    uint8_t data_buffer[BATTERY_READ_CMD_SIZE];
    uint8_t* data_received = &data_buffer;

    // start listening for a response
    i2c_masterRead(BAT_ADDRESS, BATTERY_READ_CMD_SIZE, &data_received);

    // wait until the battery responds and hence fills the buffer
    while (!i2c_getRxDoneFlag()){
      for (uint8_t i=0; i<100; i++); //delay
    }
    i2c_clearRxDoneFlag();

    // Parse
    uint8_t part1 = data_received[0];
    uint8_t part2 = data_received[1];
    uint16_t response = ( ((uint16_t)data_received[0]) << 8 ) + (uint16_t)data_received[1];

    return apply_equation_to_adc(response, reg);
}
