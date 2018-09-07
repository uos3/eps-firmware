#include "battery.h"

typedef struct bat_write_cmd_packet {
    uint8_t address:7;
    uint8_t write:1;   // unsure about this (both ordering and method of addressing)
    uint8_t cmd;
    uint8_t data[2];
} bat_w_cmd_t;

static bat_w_cmd_t bat_w_cmd;

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

//uint16_t get_battery_telemetry(uint16_t reg, uint8_t* i2c_buff){
////    bat_w_cmd.address_prompt = (BAT_ADDRESS << 1) | BAT_WRITE_CMD;
//    bat_w_cmd.write = BAT_WRITE_CMD; // warning, could be breaking due to not bit masking
//    bat_w_cmd.address = BAT_ADDRESS;
//    bat_w_cmd.cmd = 0x10;
//    bat_w_cmd.data[0] = (uint8_t)(0x00FF & reg);
//    bat_w_cmd.data[1] = (uint8_t)(reg >> 8);
//
//    // send command
//    i2c_masterWrite(bat_w_cmd.address, 4, &bat_w_cmd);
//
////    for (i=0; i<10000; i++);// wait
//
//    // send request for data buffered
//    uint8_t address_prompt = (BAT_ADDRESS << 1) | BAT_READ_CMD;
//    i2c_masterWrite(bat_w_cmd.address, 1, &address_prompt);
//
////    for (i=0; i<10000; i++);// wait
//
//    // receive and store data
//    uint8_t* received_bytes;
//        //make response
//    i2c_masterRead(bat_w_cmd.address, 2, &received_bytes);
//
//    // Parse
//    uint16_t response = ( ((uint16_t)received_bytes[0]) << 8 ) + (uint16_t)received_bytes[1];
//
//    return apply_equation_to_adc(response, reg);
//}

uint16_t get_battery_telemetry(uint16_t reg, uint8_t* i2c_buff){
//    bat_w_cmd.address_prompt = (BAT_ADDRESS << 1) | BAT_WRITE_CMD;
    bat_w_cmd.write = BAT_WRITE_CMD; // warning, could be breaking due to not bit masking
    bat_w_cmd.address = BAT_ADDRESS;
    bat_w_cmd.cmd = 0x10;
    bat_w_cmd.data[0] = (uint8_t)(0x00FF & reg);
    bat_w_cmd.data[1] = (uint8_t)(reg >> 8);

    // send command
    i2c_masterWrite(BAT_ADDRESS, 4, &bat_w_cmd);

//    for (i=0; i<10000; i++);// wait

    // send request for data buffered
    uint8_t address_prompt = (BAT_ADDRESS << 1) | BAT_READ_CMD;
    i2c_masterWrite(BAT_ADDRESS, 1, &address_prompt);

    uint8_t i = 0;
    for (i=0; i<100; i++);// wait

    // receive and store data
    uint8_t* received_bytes;
        //make response
    i2c_masterRead(bat_w_cmd.address, 2, &received_bytes);

    uint8_t part1 = received_bytes[0];
    uint8_t part2 = received_bytes[1];

    // Parse
    uint16_t response = ( ((uint16_t)received_bytes[0]) << 8 ) + (uint16_t)received_bytes[1];

    return apply_equation_to_adc(response, reg);
}
