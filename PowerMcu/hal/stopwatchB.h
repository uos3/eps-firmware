/*
 * stopwatch.h
 *
 *  Created on: 8 Jan 2016
 *      Author: Tom
 */

#ifndef HAL_STOPWATCHB_H_
#define HAL_STOPWATCHB_H_

#include <stdint.h>

#define TICKS_PER_SECOND 32768

void stopwatch_initB();
void stopwatch_disableB();
void stopwatch_processB();
void stopwatch_startB(uint32_t stopValue, void (*stopwatch_callback)());

#endif /* HAL_STOPWATCHB_H_ */
