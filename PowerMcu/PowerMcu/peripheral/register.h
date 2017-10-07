/*
 * registers.h
 *
 *  Created on: 9 Oct 2015
 *      Author: Tom
 */

#ifndef REGISTER_H_
#define REGISTER_H_

uint16_t register_get(uint8_t id);
void register_set(uint8_t id, uint16_t value);
void register_init();

#define CONFIG     0x00
#define STATUS     0x01

#define SW_ON      0x02
#define SW_ON_5    (1<<5)
#define SW_ON_4    (1<<4)
#define SW_ON_3    (1<<3)
#define SW_ON_2    (1<<2)
#define SW_ON_1    (1<<1)
#define SW_ON_0    (1<<0)

#define POWER      0x03
#define BAT_V      0x04
#define BAT_I      0x05
#define BAT_T      0x06
#define SOLAR_N1_I 0x07
#define SOLAR_N2_I 0x08
#define SOLAR_S1_I 0x09
#define SOLAR_S2_I 0x0A
#define SOLAR_E1_I 0x0B
#define SOLAR_E2_I 0x0C
#define SOLAR_W1_I 0x0D
#define SOLAR_W2_I 0x0E
#define SOLAR_T1_I 0x0F
#define SOLAR_T2_I 0x10
#define RESERVED   0x11
#define SOLAR_2_V  0x12
#define RAIL1_BOOT 0x13
#define RAIL1_FAIL 0x14
#define RAIL1_V    0x15
#define RAIL1_I    0x16
#define RAIL2_BOOT 0x17
#define RAIL2_FAIL 0x18
#define RAIL2_V    0x19
#define RAIL2_I    0x1A
#define RAIL3_BOOT 0x1B
#define RAIL3_FAIL 0x1C
#define RAIL3_V    0x1D
#define RAIL3_I    0x1E
#define RAIL4_BOOT 0x1F
#define RAIL4_FAIL 0x20
#define RAIL4_V    0x21
#define RAIL4_I    0x22
#define RAIL5_BOOT 0x23
#define RAIL5_FAIL 0x24
#define RAIL5_V    0x25
#define RAIL5_I    0x26
#define RAIL6_BOOT 0x27
#define RAIL6_FAIL 0x28
#define RAIL6_V    0x29
#define RAIL6_I    0x2A
#define SUPPLY_3_V 0x2B
#define SUPPLY_3_I 0x2C
#define SUPPLY_5_V 0x2D
#define SUPPLY_5_I 0x2E
#define CHARGE_I   0x2F
#define MPPT_BUS_V 0x30
#define CRC_ER_CNT 0x31
#define DROP_CNT   0x32

#endif /* REGISTER_H_ */
