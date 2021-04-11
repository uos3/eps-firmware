/*
 * @ingroup gpio
 *
 * @file Adc_public.c
 * @author Daniel Murphy (dm4g16@soton.ac.uk)
 * @brief Public source file for ADC driver.
 *
 * @version 0.1
 * @date 2021-03-30
 *
 * @copyright UOS3
 */
/* -------------------------------------------------------------------------
 * INCLUDES
 * ------------------------------------------------------------------------- */

/* Internal includes */
#include "Adc_public.h"

/* -------------------------------------------------------------------------
 * FUNCTIONS
 * ------------------------------------------------------------------------- */
void Adc_init(void){
    /* Sets the ADC pins, A7 for MUX1 and A6 for MUX2
     * */
    ADC10AE0 = BIT7 + BIT6;
    /* Disable ADC */
    ADC10CTL0 &= ~ENC;
    /* Sets Vref as Vcc, Sample and hold time 64x ADC10CLKs, Decreases reference buffer
     * capability to reduce current, reference output on, reference buffer only on during sample
     * and conversion, 2.5V, enables ADC and enables interrupt  */
    ADC10CTL0 |= SREF_0 + ADC10SHT_3 + ADC10SR + REFON + REFBURST + REF2_5V + ADC10ON + ADC10IE;
}

void Adc_start_conversion(uint16_t channel_in){
    /*Sets time source as ADC10OSC and selects the channel, INCH_6 for MUX 1 and INCH_7 for MUX 2*/
    ADC10CTL1 = ADC10SSEL_0 + channel_in;
    /*Delays to allow Vref to settle*/
    __delay_cycles(128);
    /* Enables ADC and starts conversion */
    ADC10CTL0 |= ENC + ADC10SC;
}

uint16_t Adc_convert(uint16_t channel_in){
    uint16_t result;
    Adc_start_conversion(channel_in);
    /*Stores the converted value */
    result = ADC10MEM;
    /*Disable ADC */
    ADC10CTL0 &= ~ENC;
    return result;
}


