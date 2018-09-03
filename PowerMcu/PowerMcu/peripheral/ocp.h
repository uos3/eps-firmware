/*
 * ocp.h
 *
 *  Created on: 6 Nov 2015
 *      Author: Tom
 */

#ifndef PERIPHERAL_OCP_H_
#define PERIPHERAL_OCP_H_

#include <stdint.h>

void ocp_process();

// Set the active state of each rail.
// value: bitfield with the state of each rail
//     0 - Off
//     1 - On (except in overcurrent state)
//     rails & 0x1 is for OCP0
//     rails & 0x2 is for OCP1 etc
void ocp_activate_rail(uint8_t value);

// Gets the active state of each rail.
// Return value: bitfield with the state of each rail
//     0 - Off
//     1 - On (except in overcurrent state)
//     rails & 0x1 is for OCP0
//     rails & 0x2 is for OCP1 etc
uint8_t ocp_get_rails_activated();

uint16_t ocp_get_boots(uint8_t railIndex);
uint16_t ocp_get_fails(uint8_t railIndex);

#endif /* PERIPHERAL_OCP_H_ */
