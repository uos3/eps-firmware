/*
 * @ingroup gpio
 *
 * @file Adc_public.h
 * @author Daniel Murphy (dm4g16@soton.ac.uk)
 * @brief Public header for ADC driver.
 *
 * @version 0.1
 * @date 2021-03-30
 *
 * @copyright UOS3
 */

#ifndef DRIVERS_GPIO_ADC_ADC_PUBLIC_H_
#define DRIVERS_GPIO_ADC_ADC_PUBLIC_H_

/* -------------------------------------------------------------------------
 * INCLUDES
 * ------------------------------------------------------------------------- */

/* Standard library includes */
#include <stdio.h>
#include <stdint.h>
#include <msp430.h>

/* -------------------------------------------------------------------------
 * DEFINES
 * ------------------------------------------------------------------------- */

#define ADC_CHANNEL_MUX1 INCH_7
#define ADC_CHANNEL_MUX2 INCH_6
#define ADC_TEMP_SENSOR INCH_10

/* -------------------------------------------------------------------------
 * FUNCTIONS
 * ------------------------------------------------------------------------- */
void Adc_init(void);
void Adc_start_conversion(uint16_t channel_in);
uint16_t Adc_convert(uint16_t channel_in);




#endif /* DRIVERS_GPIO_ADC_ADC_PUBLIC_H_ */
