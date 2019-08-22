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
uint16_t convert_adc_to_current(uint16_t csense, uint16_t shunt_resistance);

#define CONFIG     0x00         // There is an interrupt line pulled low when an overcurrent event occurs, this can be used to ignore overcurrents on various lines through use of a mask as well as packet error interrupts
#define STATUS     0x01         //This shows which lines have experienced an OCP event, top bit indicates packet timeout event, second indicates crc event

#define SW_ON      0x02         //ID's
#define SW_ON_5    (1<<5)       //LNA           Each bit in byte sent to SW_ON corresponds with a rail and state (on or off)
#define SW_ON_4    (1<<4)       //GPS Rx
#define SW_ON_3    (1<<3)       //Flight MCU
#define SW_ON_2    (1<<2)       //Cam
#define SW_ON_1    (1<<1)       //Rx Ground Comms
#define SW_ON_0    (1<<0)       //Tx Ground Comms
#define ALL_RAILS  63

#define RESET      0x03         //Up to 128 registers allowed by by comms packets address space
#define BAT_V      0x04
#define BAT_I      0x05
#define BAT_T      0x06
#define SOLAR_POS_X1_I 0x07
#define SOLAR_POS_X2_I 0x08
#define SOLAR_NEG_X1_I 0x09
#define SOLAR_NEG_X2_I 0x0A
#define SOLAR_POS_Y1_I 0x0B
#define SOLAR_POS_Y2_I 0x0C
#define SOLAR_NEG_Y1_I 0x0D
#define SOLAR_NEG_Y2_I 0x0E
#define SOLAR_NEG_Z1_I 0x0F
#define SOLAR_NEG_Z2_I 0x10
#define SOLAR_1_V  0x11
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
#define SW_OFF     0x33
#define KNOWN_VAL  0x34

#endif /* REGISTER_H_ */
