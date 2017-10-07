/*
 * circ.h
 *
 * Created: 27/10/2015 17:30:28
 *  Author: Tom
 */


#ifndef CIRC_H_
#define CIRC_H_

#include <stdint.h>

#define CIRC_MAX 64

typedef enum
{
	PUSH_FULL = 0x1,
	POP_EMPTY = 0x2
} CIRC_ERROR;

typedef struct
{
	volatile uint8_t buffer[CIRC_MAX];
	volatile uint8_t start;
	volatile uint8_t end;
	volatile CIRC_ERROR err;
} circ;

void circ_init(circ *c);
int circ_push(circ *c, uint8_t value);
int circ_pop(circ *c);
int circ_peek(circ *c, int offset);
int circ_length(circ *c);
CIRC_ERROR circ_get_error(circ *c);
void circ_clear_error(circ *c);

#endif /* CIRC_H_ */