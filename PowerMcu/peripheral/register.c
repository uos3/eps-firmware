/*
 * register.c
 *
 *  Created on: 11 Oct 2015
 *      Author: Tom
 */

#include <peripheral/errormon.h>
#include "hal/switches.h"
#include "peripheral/battery.h"
#include "register.h"
#include "sense.h"
#include "status.h"
#include "interrupt.h"
#include "ocp.h"

uint8_t* i2c_buff;


void register_init()
{
	register_set(CONFIG, 0x100);
	register_set(STATUS, 0xFF); // 0xFF clears any flags.
	register_set(SW_OFF, ALL_RAILS);
	register_set(CRC_ER_CNT, 0);
	register_set(DROP_CNT, 0);
}

uint16_t register_get(uint8_t id)
{
	switch(id)
	{
	case CONFIG:/*0*/return interrupt_get_mask();
	case STATUS:/*1*/return status_get();
	case SW_ON: /*2*/return ocp_get_rails_activated();
	case POWER: /*3*/return ocp_get_rails_activated();              //Unnecessary

	case BAT_V: /*4*/return sense_get(VBATT_VSENSE);
    case BAT_I:      return get_battery_telemetry(BAT_I2C_IBAT, i2c_buff);
    case BAT_T:      return get_battery_telemetry(BAT_I2C_TEMPBRD, i2c_buff);

	case SOLAR_POS_X1_I: return sense_get(PV_NORTH1_CSENSE_MUX);
	case SOLAR_POS_X2_I: return sense_get(PV_NORTH2_CSENSE_MUX);
	case SOLAR_NEG_X1_I: return sense_get(PV_SOUTH1_CSENSE_MUX);
	case SOLAR_NEG_X2_I: return sense_get(PV_SOUTH2_CSENSE_MUX);
	case SOLAR_POS_Y1_I: return sense_get(PV_EAST1_CSENSE_MUX);
	case SOLAR_POS_Y2_I: return sense_get(PV_EAST2_CSENSE_MUX);
	case SOLAR_NEG_Y1_I: return sense_get(PV_WEST1_CSENSE_MUX);
	case SOLAR_NEG_Y2_I: return sense_get(PV_WEST2_CSENSE_MUX);
	case SOLAR_NEG_Z1_I: return sense_get(PV_TOP1_CSENSE_MUX);
	case SOLAR_NEG_Z2_I: return sense_get(PV_TOP2_CSENSE_MUX);
    case SOLAR_1_V:  return sense_get(MPPT1_PV_VSENSE);
	case SOLAR_2_V:  return sense_get(MPPT2_PV_VSENSE);

	case RAIL1_BOOT: return ocp_get_boots(0);
	case RAIL1_FAIL: return ocp_get_fails(0);
	case RAIL1_V:    return sense_get(OCP1_VSENSE);
	case RAIL1_I:    return convert_adc_to_current(sense_get(OCP1_CSENSE_MUX), 50);

	case RAIL2_BOOT: return ocp_get_boots(1);
	case RAIL2_FAIL: return ocp_get_fails(1);
	case RAIL2_V:    return sense_get(OCP2_VSENSE);
	case RAIL2_I:    return convert_adc_to_current(sense_get(OCP2_CSENSE_MUX), 124);

    case RAIL3_BOOT: return ocp_get_boots(2);
    case RAIL3_FAIL: return ocp_get_fails(2);
	case RAIL3_V:    return sense_get(OCP3_VSENSE);
	case RAIL3_I:    return convert_adc_to_current(sense_get(OCP3_CSENSE_MUX), 124);

    case RAIL4_BOOT: return ocp_get_boots(3);
    case RAIL4_FAIL: return ocp_get_fails(3);
	case RAIL4_V:    return sense_get(OCP4_VSENSE);
	case RAIL4_I:    return convert_adc_to_current(sense_get(OCP4_CSENSE_MUX), 442);

    case RAIL5_BOOT: return ocp_get_boots(4);
    case RAIL5_FAIL: return ocp_get_fails(4);
	case RAIL5_V:    return sense_get(OCP5_VSENSE);
	case RAIL5_I:    return convert_adc_to_current(sense_get(OCP5_CSENSE_MUX), 50);

    case RAIL6_BOOT: return ocp_get_boots(5);
    case RAIL6_FAIL: return ocp_get_fails(5);
	case RAIL6_V:    return sense_get(OCP6_VSENSE);
	case RAIL6_I:    return convert_adc_to_current(sense_get(OCP6_CSENSE_MUX), 124);

	case SUPPLY_3_V: return sense_get(UVP3V3_VSENSE);
	case SUPPLY_3_I: return sense_get(SYS_3V3_CSENSE_MUX);
	case SUPPLY_5_V: return sense_get(UVP5V_VSENSE);
	case SUPPLY_5_I: return sense_get(SYS_5V_CSENSE_MUX);

	case CHARGE_I:   return sense_get(CHARGE_CSENSE_MUX);
	case MPPT_BUS_V: return sense_get(MPPT_BUS_VSENSE);

	case CRC_ER_CNT: return errormon_get(CrcInvalid);
	case DROP_CNT:   return errormon_get(PacketTimeout);

	case SW_OFF:     return ocp_get_rails_activated();
	case KNOWN_VAL:  return 0x42;

	default:
		return 0;
	}
}

//Note: SW_ON toggles rails rather than switching on or off
void register_set(uint8_t id, uint16_t value)
{
	switch(id)
	{
	case CONFIG: interrupt_set_mask((uint8_t) value); break;   //
	case STATUS: status_clear_bits((uint8_t) value); break;
	case SW_ON:  ocp_toggle_rail(value); break;
    case SW_OFF:  ocp_deactivate_all_rails(); break;
	case CRC_ER_CNT: errormon_set(CrcInvalid, value); break;
	case DROP_CNT: errormon_set(PacketTimeout, value); break;
	}
	return;
}

// Due to using integers, all division is left to the end
uint16_t convert_adc_to_current(uint16_t csense, uint16_t shunt_resistance){
    // Convert current from A to mA, and also shunt resistance from Ohms to mOhms

    // De-amplify the gain of INA214DCK (current monitor);
    uint32_t current = csense * 2.5;


    current *= 1000;
    current *= 1000;
    current /= 1024;
    current /= 100;
    current /= shunt_resistance;



    // converting from adc to 0 to 2.5V range, but in percent (so from 0.0 to 1.0)

    // Work out current flowing through shunt resistor using V=IR



    return (uint16_t)current;
}
