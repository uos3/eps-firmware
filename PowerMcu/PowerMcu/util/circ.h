/*
 * circ.h
 *
 *  Created on: 9 Oct 2015
 *      Author: Tom
 */

#ifndef UTIL_CIRC_H_
#define UTIL_CIRC_H_

#include <stdint.h>

#define CIRC_MAX 64

typedef struct
{
	volatile uint8_t buffer[CIRC_MAX];
	volatile uint8_t start;
	volatile uint8_t end;
} circ;

void circ_init(circ *c);
int circ_push(circ *c, uint8_t value);
int circ_pop(circ *c);
int circ_length(circ *c);

#endif /* UTIL_CIRC_H_ */
