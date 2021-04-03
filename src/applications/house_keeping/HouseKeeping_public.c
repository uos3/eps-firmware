/*
 *
 * @file HouseKeeping_public.c
 * @author Roberto Holmes (rh3u17@soton.ac.uk / robertoholmes@live.com)
 * @brief Get all the data available to the EPS so that it can be
 * transmitted to the TOBC.
 *
 * @version 0.1
 * @date 2021-04-03
 *
 * @copyright UOS3
 */

#include "HouseKeeping_public.h"

int HouseKeeping_get_data(uint8_t *p_packet_out) {
    uint8_t i;
    uint16_t temp_collected_data;

    /* Get battery data */

    /* Battery status */
    temp_collected_data = BatteryComms_TX_RX(0x0100);
    p_packet_out[0] = (uint8_t) temp_collected_data >> 4;
    p_packet_out[1] = (uint8_t) temp_collected_data;

    /* Battery ADCs (15 values available)*/
    for (i = 0; i < 15; i++) {
        /* Get value from ith ADC */
        temp_collected_data = BatteryComms_TX_RX((uint16_t) i);

        /* Append it to the packet */
        p_packet_out[i + i + 2u] = (uint8_t) temp_collected_data >> 4;
        p_packet_out[i + i + 3u] = (uint8_t) temp_collected_data;
    }

    /* Get data from each rail */
    Rails_get_data(&p_packet_out[33]);

    /* Read the Log File */
    LogFile_read(&p_packet_out[106]);

    /* Clear log now that it has been read */
    LogFile_clear();

    return 0;
}

