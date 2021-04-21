/*
 *
 * @file Battery_public.c
 * @author Roberto Holmes (rh3u17@soton.ac.uk / robertoholmes@live.com)
 * @brief Public Battery component source file
 *
 * Hold functions to transmit to and receive from the battery.
 *
 * Task ref: [EPS_SW_1.6.2]
 *
 * @version 0.1
 * @date 2021-04-09
 *
 * @copyright UOS3
 */

#include "Battery_public.h"

uint8_t Battery_TX(uint8_t command_in, uint16_t data_in) {

    /* Make first byte of the packet the type of battery command */
    BATTERY_PACKET[0] = command_in;

    /* If telemetry is being requested, make the second byte.
     * Other requests only require 1 data byte */
    if (BATTERY_PACKET[0] == 0x10) {
        BATTERY_PACKET[1] = (uint8_t) (data_in >> 8);
        BATTERY_PACKET[2] = (uint8_t) data_in;
        return I2c_master_write(BATTERY_I2C_ADDR, BATTERY_PACKET_MAX_LENGTH,
                                BATTERY_PACKET);
    }
    else {
        BATTERY_PACKET[1] = (uint8_t) data_in;
        return I2c_master_write(BATTERY_I2C_ADDR, BATTERY_PACKET_MAX_LENGTH - 1,
                                BATTERY_PACKET);
    }
}
;

uint16_t Battery_RX() {
    uint8_t valid_packet_check;
    /* Get data from the I2C bus and store any error codes */
    valid_packet_check = I2c_master_read(BATTERY_I2C_ADDR,
    BATTERY_RX_PACKET_LENGTH,
                                         BATTERY_PACKET);

    /* Only return collected data if no error occurred */
    if (valid_packet_check == 0) {
        return ((BATTERY_PACKET[0] << 8) | BATTERY_PACKET[1]);
    }
    else {
        return 0xFFFF;
    }

}
;

