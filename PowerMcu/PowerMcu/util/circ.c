/*
 * circ.c
 *
 *  Created on: 9 Oct 2015
 *      Author: Tom
 */

#include "circ.h"

void circ_init(circ *c)
{
	c->start = 0;
	c->end = 0;
}

int circ_push(circ *c, uint8_t value)
{
	if (circ_length(c) < CIRC_MAX - 1)
	{
		uint8_t end = c->end;
		c->buffer[end] = value;
		c->end = (end + 1) % CIRC_MAX;
		return 1;
	}
	else
		return 0;
}

int circ_pop(circ *c)
{
	uint8_t start = c->start;
	if (start != c->end)
	{
		uint8_t result = c->buffer[start];
		c->start = (start + 1) % CIRC_MAX;
		return result;
	}
	else
		return -1;
}

int circ_length(circ *c)
{
	uint8_t end = c->end;
	uint8_t start = c->start;
	if (end < start)
		return CIRC_MAX - start + end;
	else
		return end - start;
}


