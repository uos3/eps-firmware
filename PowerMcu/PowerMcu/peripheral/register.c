/*
 * register.c
 *
 *  Created on: 11 Oct 2015
 *      Author: Tom
 */

#include <peripheral/errormon.h>
#include "hal/switches.h"
#include "register.h"
#include "sense.h"
#include "status.h"
#include "interrupt.h"
#include "ocp.h"

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
	case POWER: /*3*/return switches_get();

	case BAT_V: /*4*/return sense_get(VBATT_VSENSE);
	case BAT_I:      return 0; // TODO: Implement over I2C to battery board.
	case BAT_T:      return 0; // TODO: Implement over I2C to battery board.

	case SOLAR_N1_I: return sense_get(PV_NORTH1_CSENSE_MUX);
	case SOLAR_N2_I: return sense_get(PV_NORTH2_CSENSE_MUX);
	case SOLAR_S1_I: return sense_get(PV_SOUTH1_CSENSE_MUX);
	case SOLAR_S2_I: return sense_get(PV_SOUTH2_CSENSE_MUX);
	case SOLAR_E1_I: return sense_get(PV_EAST1_CSENSE_MUX);
	case SOLAR_E2_I: return sense_get(PV_EAST2_CSENSE_MUX);
	case SOLAR_W1_I: return sense_get(PV_WEST1_CSENSE_MUX);
	case SOLAR_W2_I: return sense_get(PV_WEST2_CSENSE_MUX);
	case SOLAR_T1_I: return sense_get(PV_TOP1_CSENSE_MUX);
	case SOLAR_T2_I: return sense_get(PV_TOP2_CSENSE_MUX);
	case SOLAR_2_V:  return sense_get(MPPT2_PV_VSENSE);

	case RAIL1_BOOT: return ocp_get_boots(0);
	case RAIL1_FAIL: return ocp_get_fails(0);
	case RAIL1_V:    return sense_get(OCP1_VSENSE);
	case RAIL1_I:    return convert_adc_to_current(sense_get(OCP1_CSENSE_MUX), 50);

	case RAIL2_BOOT: return ocp_get_boots(1);
	case RAIL2_FAIL: return ocp_get_fails(1);
	case RAIL2_V:    return sense_get(OCP2_VSENSE);
	case RAIL2_I:    return convert_adc_to_current(sense_get(OCP2_CSENSE_MUX), 124);

	case RAIL3_V:    return sense_get(OCP3_VSENSE);
	case RAIL3_I:    return convert_adc_to_current(sense_get(OCP3_CSENSE_MUX), 124);

	case RAIL4_V:    return sense_get(OCP4_VSENSE);
	case RAIL4_I:    return convert_adc_to_current(sense_get(OCP4_CSENSE_MUX), 442);

	case RAIL5_V:    return sense_get(OCP5_VSENSE);
	case RAIL5_I:    return convert_adc_to_current(sense_get(OCP5_CSENSE_MUX), 50);

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

	case SW_OFF: /*2*/return ocp_get_rails_activated();

	default:
		return 0;
	}
}

void register_set(uint8_t id, uint16_t value)
{
	switch(id)
	{
	case CONFIG: interrupt_set_mask((config_t) value); break;
	case STATUS: status_clear_bits((status_t) value); break;
	case SW_ON:  ocp_activate_rail(value); break;
    case SW_OFF:  ocp_deactivate_rail(value); break;
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
