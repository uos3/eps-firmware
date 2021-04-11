/*
 *
 * @file Rails_public.c
 * @author Roberto Holmes (rh3u17@soton.ac.uk / robertoholmes@live.com)
 * @brief Public Rails component source file.
 *
 * Modifies the registers relevant to the OCP rails and monitors
 * the status of all the rails.
 *
 * Task ref: [EPS_SW_1.6.3]
 *
 * @version 0.1
 * @date 2021-04-10
 *
 * @copyright UOS3
 */

#include "Rails_public.h"

void Rails_init() {
    /* Configure OCP pins as inputs */
    P1DIR &= ~(RAILS_OCP1_PIN | RAILS_OCP2_PIN | RAILS_OCP3_PIN | RAILS_OCP4_PIN
            | RAILS_OCP5_PIN | RAILS_OCP6_PIN);
    /* Set resistors to pull up if enabled */
    P1OUT |= RAILS_OCP1_PIN | RAILS_OCP2_PIN | RAILS_OCP3_PIN | RAILS_OCP4_PIN
            | RAILS_OCP5_PIN | RAILS_OCP6_PIN;

}

/* Add all data available through the multiplexers to the packet */
uint8_t Rails_get_data(uint8_t *p_packet_out) {
    uint8_t j, i = 0;

    /* Iterate through all 32 MUX1 channels */
    for (j = 0; j < 32; j++) {
        /* Avoiding channel 16 which is not connected */
        if (j == 16) {
            /* Select pin on the MUX and only continue if it is successful */
            if (Mux_select(j) == 0) {
                /* Convert the data and append it to the packet */
                i = Rails_append_adc_data(Adc_convert(ADC_CHANNEL_MUX1),
                                          p_packet_out, i);
            }
        }
    }

    /* Iterate through all 4 MUX2 channels */
    for (j = 0; j < 4; j++) {
        /* Select pin on the MUX and only continue if it is successful */
        if (Mux2_select(j) == 0) {
            /* Convert the data and append it to the packet */
            i = Rails_append_adc_data(Adc_convert(ADC_CHANNEL_MUX2),
                                      p_packet_out, i);
        }
    }
    return 0;
}

/* Turn the rail in the ith bit of the rail_num_in byte off if new_state_in = 0
 * and turn on otherwise
 */
void Rails_set(uint8_t rail_num_in, uint8_t new_state_in) {
    uint8_t rail = Rails_convert_rail(rail_num_in);
    if (new_state_in) {
        /* Disable pull up resistor */
        P1REN &= ~rail;
        /* Enable interrupt */
        P1IE |= rail;
        /* Set interrupt on rising edge */
        P1IES &= ~rail;
        /* Clear interrupt flag */
        P1IFG &= ~rail;
    }
    else {
        /* Disable interrupt */
        P1IE &= ~rail;
        /* Enable pull up resistor */
        P1REN |= rail;
    }

}

/* Convert the rail stored as the ith bit in a byte to the byte corresponding
 * to the pin on the P1 bank of the microcontroller
 */
uint8_t Rails_convert_rail(uint8_t rail_num_in) {
    switch (rail_num_in) {
    case BIT1:
        return RAILS_OCP1_PIN;
    case BIT2:
        return RAILS_OCP2_PIN;
    case BIT3:
        return RAILS_OCP3_PIN;
    case BIT4:
        return RAILS_OCP4_PIN;
    case BIT5:
        return RAILS_OCP5_PIN;
    case BIT6:
        return RAILS_OCP6_PIN;
    }
    return 0;
}

/* Add the ADC data the output packet so that the Most Significant Byte
 * of the 2 byte ADC value is first */
uint8_t Rails_append_adc_data(uint16_t data_in, uint8_t *p_packet_out,
                              uint8_t i_inout) {
    p_packet_out[i_inout + i_inout] = (uint8_t) data_in >> 8;
    p_packet_out[i_inout + i_inout + 1u] = (uint8_t) data_in;
    return i_inout++;
}
