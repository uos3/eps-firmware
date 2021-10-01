/*
 *
 * @file BatteryComms_public.c
 * @author Roberto Holmes (rh3u17@soton.ac.uk / robertoholmes@live.com)
 * @brief Handle requests to communicate with the battery.
 *
 * @version 0.1
 * @date 2021-04-02
 *
 * @copyright UOS3
 */

#include "BatteryComms_public.h"

/* Transmit given command to the battery and then get the response */
uint16_t BatteryComms_TX_RX(uint8_t command_in, uint16_t data_in) {
    Battery_TX(command_in, data_in);

//hello my name is Oli and this a git test
    /* Only expect a response if a manual reset was not ordered */
    if (command_in != 0x80) {
        return Battery_RX();
    }
    return 0;
}

