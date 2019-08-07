/*
 * intpin.h
 *
 *  Created on: 31 Oct 2015
 *      Author: Tom
 */

#ifndef HAL_INTPIN_H_
#define HAL_INTPIN_H_

#include <stdint.h>

void intpin_init();
void intpin_set(int interruptActive);

#endif /* HAL_INTPIN_H_ */
