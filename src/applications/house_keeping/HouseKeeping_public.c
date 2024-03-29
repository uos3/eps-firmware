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
    /* Debugging */
//    uint8_t i;
//    for(i=0;i<105;i++){
//        p_packet_out[i]=i;
//    }
    /* Get battery status and ADC values */
    HouseKeeping_get_bat_data(p_packet_out);

    /* Read the temperature sensor and append the data to the packet */
    Convert_16bit_to_8bit(Adc_convert(ADC_TEMP_SENSOR), p_packet_out, 22);

    /* Get data from each rail (Bytes 24 - 95)*/
    Rails_get_data(&p_packet_out[24]);

    /* Read the Log File (Bytes 96 - 104)  and whether it is valid */
    p_packet_out[96] = LogFile_read(&p_packet_out[97]);

    /* Clear log now that it has been read */
    LogFile_clear();

    p_packet_out[105] = RAILS_CURRENT_STATE;

    return 0;
}

