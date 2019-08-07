/*
 * switches.h
 *
 *  Created on: 11 Oct 2015
 *      Author: Tom
 */

#ifndef SWITCHES_H_
#define SWITCHES_H_

#include <stdint.h>

void switches_init();
uint8_t switches_get();

uint8_t switches_get_flags();

// For each pin in bitfield mask:
//     0 is input
//     1 is output
void switches_set_direction(uint8_t mask);
uint8_t switches_get_direction();

#endif /* SWITCHES_H_ */
