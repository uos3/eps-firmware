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
    /* Get battery status and ADC values (Bytes 0 - 31)*/
    HouseKeeping_get_bat_data(p_packet_out);

    /* Get data from each rail (Bytes 32 - 104)*/
    Rails_get_data(&p_packet_out[33]);

    /* Read the Log File (Bytes 105 - 112)*/
    LogFile_read(&p_packet_out[106]);

    /* Clear log now that it has been read */
    LogFile_clear();

    return 0;
}



