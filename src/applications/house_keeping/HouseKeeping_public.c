/*
 *
 * @file HouseKeeping_public.c
 * @author Roberto Holmes (rh3u17@soton.ac.uk / robertoholmes@live.com)
 * @brief Public House Keeping source file
 *
 * Get all the data available to the EPS so that it can be
 * transmitted to the TOBC.
 *
 * Task ref: [EPS_SW_1.5.3]
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

    /* Read the temperature sensor and append the data to the packet */
    Rails_append_adc_data(Adc_convert(ADC_TEMP_SENSOR), p_packet_out, 32);

    /* Get data from each rail (Bytes 34 - 103)*/
    Rails_get_data(&p_packet_out[34]);

    /* Read the Log File (Bytes 104 - 111)*/
    LogFile_read(&p_packet_out[104]);

    /* Clear log now that it has been read */
    LogFile_clear();

    return 0;
}



