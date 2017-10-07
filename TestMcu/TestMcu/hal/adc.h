/*
 * adc.h
 *
 * Created: 01/11/2015 16:47:27
 *  Author: Tom
 */


#ifndef ADH_H_
#define ADH_H_

#include <stdint.h>

void adc_init();
void adc_start();
// Return value is 0 if there are no results ready,
// otherwise returns a pointer to the conversion results.
uint16_t *adc_try_get_results();

#endif /* ADH_H_ */