/*
 * Circular_buffer_public.h
 *
 *  Created on: 18 Mar 2021
 *      Author: danielmurphy
 */

#ifndef UTIL_CIRCULAR_BUFFERS_CIRCULAR_BUFFER_PUBLIC_H_
#define UTIL_CIRCULAR_BUFFERS_CIRCULAR_BUFFER_PUBLIC_H_

#include <stdint.h>

#define CIRC_MAX (64)

typedef struct
{
    volatile uint8_t buffer[CIRC_MAX];
    volatile uint8_t start;
    volatile uint8_t end;
} circ;

void Circular_buffer_init(circ *p_c);
uint8_t Circular_buffer_push(circ *p_c, uint8_t value);
uint8_t Circular_buffer_pop(circ *p_c);



#endif /* UTIL_CIRCULAR_BUFFERS_CIRCULAR_BUFFER_PUBLIC_H_ */
