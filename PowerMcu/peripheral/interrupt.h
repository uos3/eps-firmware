/*
 * interrupt.h
 *
 *  Created on: 31 Oct 2015
 *      Author: Tom
 */

#ifndef INTERRUPT_H_
#define INTERRUPT_H_

#include <stdint.h>

uint8_t interrupt_get_mask();
void interrupt_set_mask(uint8_t value);
void interrupt_set_status(uint8_t status);

#endif /* INTERRUPT_H_ */
