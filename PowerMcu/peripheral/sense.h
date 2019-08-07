/*
 * sense.h
 *
 *  Created on: 23 Oct 2015
 *      Author: Tom
 */

#ifndef SENSE_H_
#define SENSE_H_

#include <stdint.h>

#define PV_TOP1_CSENSE_MUX   0
#define PV_TOP2_CSENSE_MUX   1
#define OCP5_CSENSE_MUX      2
#define OCP6_CSENSE_MUX      3
#define OCP6_VSENSE          4
#define OCP4_CSENSE_MUX      5
#define OCP4_VSENSE          6
#define OCP5_VSENSE          7
#define OCP3_CSENSE_MUX      8
#define OCP3_VSENSE          9
#define SYS_5V_CSENSE_MUX    10
#define SYS_3V3_CSENSE_MUX   11
#define OCP2_CSENSE_MUX      12
#define OCP2_VSENSE          13
#define OCP1_VSENSE          14
#define MUX_IN1              15
#define PV_NORTH2_CSENSE_MUX 16
#define PV_NORTH1_CSENSE_MUX 17
#define CHARGE_CSENSE_MUX    18
#define PV_WEST1_CSENSE_MUX  19
#define MPPT_BUS_VSENSE      20
#define MPPT1_PV_VSENSE      21
#define MPPT2_PV_VSENSE      22
#define PV_WEST2_CSENSE_MUX  23
#define PV_SOUTH2_CSENSE_MUX 24
#define PV_SOUTH1_CSENSE_MUX 25
#define UVP5V_VSENSE         26
#define UVP3V3_VSENSE        27
#define VBATT_VSENSE         28
#define OCP1_CSENSE_MUX      29
#define PV_EAST1_CSENSE_MUX  30
#define PV_EAST2_CSENSE_MUX  31

uint16_t sense_get(uint8_t channel);


#endif /* SENSE_H_ */
