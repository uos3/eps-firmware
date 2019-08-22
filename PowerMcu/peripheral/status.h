/*
 * status.h
 *
 *  Created on: 31 Oct 2015
 *      Author: Tom
 */

#ifndef STATUS_H_
#define STATUS_H_

#define CRCBit 0x40       //Sets the top 2 if there has been a packet errors found in errormon
#define PTBit 0x80
#include <stdint.h>

void status_set_bits(uint8_t mask);
void status_clear_bits(uint8_t mask);
uint8_t status_get();

#endif /* STATUS_H_ */
