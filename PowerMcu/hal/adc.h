/*
 * adc.h
 *
 *  Created on: 23 Oct 2015
 *      Author: Tom
 */

#ifndef HAL_ADC_H_
#define HAL_ADC_H_

#include <stdint.h>

void adc_init();
uint16_t adc_get_last_result();
void adc_start_conversion(uint16_t ch);
uint16_t adc_convert(uint16_t ch);

#endif /* HAL_ADC_H_ */
