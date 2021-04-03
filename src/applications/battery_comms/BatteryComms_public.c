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
uint16_t BatteryComms_TX_RX(uint16_t command) {
    Battery_TX(command);
    return Battery_RX();
}

