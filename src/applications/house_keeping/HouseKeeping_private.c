/*
 *
 * @file HouseKeeping_private.c
 * @author Roberto Holmes (rh3u17@soton.ac.uk / robertoholmes@live.com)
 * @brief Store additional functions required by the house keeping app.
 *
 * @version 0.1
 * @date 2021-04-09
 *
 * @copyright UOS3
 */

#include "HouseKeeping_private.h"

/* Add the ADC data to bytes 0 - 31 for i values of 0-15 of the output packet */
uint8_t HouseKeeping_append_adc_data(uint16_t data_in, uint8_t *p_packet_out,
                                     uint8_t i_inout) {
    p_packet_out[i_inout + i_inout] = (uint8_t) data_in >> 8;
    p_packet_out[i_inout + i_inout + 1u] = (uint8_t) data_in;
    return i_inout++;
}

/* Go through a battery status request and then each telemetry code and append the data to the packet */
void HouseKeeping_get_bat_data(uint8_t *p_packet_out) {
    uint8_t i = 0;

    /* Get Battery Output Voltage */
    i = HouseKeeping_append_adc_data(BatteryComms_TX_RX(0x01, 0), p_packet_out,
                                     i);

    /* Get Battery Output Voltage */
    i = HouseKeeping_append_adc_data(
            BatteryComms_TX_RX(0x10, HOUSE_KEEPING_TLM_VBAT), p_packet_out, i);

    /* Get Battery Current Magnitude */
    i = HouseKeeping_append_adc_data(
            BatteryComms_TX_RX(0x10, HOUSE_KEEPING_TLM_IBAT), p_packet_out, i);

    /* Get Battery Current Direction */
    i = HouseKeeping_append_adc_data(
            BatteryComms_TX_RX(0x10, HOUSE_KEEPING_TLM_IDIRBAT), p_packet_out,
            i);

    /* Get Motherboard Temperature */
    i = HouseKeeping_append_adc_data(
            BatteryComms_TX_RX(0x10, HOUSE_KEEPING_TLM_TBRD), p_packet_out, i);

    /* Get Current Draw of 5V Bus */
    i = HouseKeeping_append_adc_data(
            BatteryComms_TX_RX(0x10, HOUSE_KEEPING_TLM_IPCM5V), p_packet_out,
            i);

    /* Get Output Voltage of 5V Bus */
    i = HouseKeeping_append_adc_data(
            BatteryComms_TX_RX(0x10, HOUSE_KEEPING_TLM_VPCM5V), p_packet_out,
            i);

    /* Get Current Draw of 3.3V Bus */
    i = HouseKeeping_append_adc_data(
            BatteryComms_TX_RX(0x10, HOUSE_KEEPING_TLM_IPCM3V3), p_packet_out,
            i);

    /* Get Output Voltage of 3.3V Bus */
    i = HouseKeeping_append_adc_data(
            BatteryComms_TX_RX(0x10, HOUSE_KEEPING_TLM_VPCM3V3), p_packet_out,
            i);

    /* Get Daughterboard 1 Temperature */
    i = HouseKeeping_append_adc_data(
            BatteryComms_TX_RX(0x10, HOUSE_KEEPING_TLM_TBAT1), p_packet_out, i);

    /* Get Daughterboard 1 Heater Status */
    i = HouseKeeping_append_adc_data(
            BatteryComms_TX_RX(0x10, HOUSE_KEEPING_TLM_HBAT1), p_packet_out, i);

    /* Get Daughterboard 2 Temperature */
    i = HouseKeeping_append_adc_data(
            BatteryComms_TX_RX(0x10, HOUSE_KEEPING_TLM_TBAT2), p_packet_out, i);

    /* Get Daughterboard 2 Heater Status */
    i = HouseKeeping_append_adc_data(
            BatteryComms_TX_RX(0x10, HOUSE_KEEPING_TLM_HBAT2), p_packet_out, i);

    /* Get Daughterboard 3 Temperature */
    i = HouseKeeping_append_adc_data(
            BatteryComms_TX_RX(0x10, HOUSE_KEEPING_TLM_TBAT3), p_packet_out, i);

    /* Get Daughterboard 3 Heater Status */
    i = HouseKeeping_append_adc_data(
            BatteryComms_TX_RX(0x10, HOUSE_KEEPING_TLM_HBAT3), p_packet_out, i);
}
