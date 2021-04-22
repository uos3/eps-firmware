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
    /* Disable ADC */
    ADC10CTL0 &= ~ENC;
    /* Sets Vref as Vcc, Sample and hold time 64x ADC10CLKs, Decreases reference buffer
     * capability to reduce current, reference output on, reference buffer only on during sample
     * and conversion enables ADC  */
    ADC10CTL0 |= SREF_0 + REFBURST + ADC10SHT_3 + REFON + ADC10ON;
    ADC10CTL1 = ADC10DIV_3;
}


uint16_t Adc_convert(uint16_t channel_in){
    ADC10AE0 = channel_in;
    ADC10CTL1 = channel_in;
    __delay_cycles(1000);
    /* Enables ADC and starts conversion */
    ADC10CTL0 |= ENC + ADC10SC;
    uint16_t result;
    while(ADC10CTL1 & BUSY);
    /*Stores the converted value */
    result = ADC10MEM;
    /*Disable ADC */
    ADC10CTL0 &= ~ENC;
    return result;
}


