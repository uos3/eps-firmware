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
	register_set(SW_ON, 0);
	register_set(CRC_ER_CNT, 0);
	register_set(DROP_CNT, 0);
}

uint16_t register_get(uint8_t id)
{
	switch(id)
	{
	case CONFIG:     return interrupt_get_mask();
	case STATUS:     return status_get();
	case SW_ON:      return ocp_get_rails_activated();
	case POWER:      return switches_get();

	case BAT_V:      return sense_get(VBATT_VSENSE);
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
	case RAIL1_I:    return sense_get(OCP1_CSENSE_MUX);

	case RAIL2_BOOT: return ocp_get_boots(1);
	case RAIL2_FAIL: return ocp_get_fails(1);
	case RAIL2_V:    return sense_get(OCP2_VSENSE);
	case RAIL2_I:    return sense_get(OCP2_CSENSE_MUX);

	case RAIL3_V:    return sense_get(OCP3_VSENSE);
	case RAIL3_I:    return sense_get(OCP3_CSENSE_MUX);

	case RAIL4_V:    return sense_get(OCP4_VSENSE);
	case RAIL4_I:    return sense_get(OCP4_CSENSE_MUX);

	case RAIL5_V:    return sense_get(OCP5_VSENSE);
	case RAIL5_I:    return sense_get(OCP5_CSENSE_MUX);

	case RAIL6_V:    return sense_get(OCP6_VSENSE);
	case RAIL6_I:    return sense_get(OCP6_CSENSE_MUX);

	case SUPPLY_3_V: return sense_get(UVP3V3_VSENSE);
	case SUPPLY_3_I: return sense_get(SYS_3V3_CSENSE_MUX);
	case SUPPLY_5_V: return sense_get(UVP5V_VSENSE);
	case SUPPLY_5_I: return sense_get(SYS_5V_CSENSE_MUX);

	case CHARGE_I:   return sense_get(CHARGE_CSENSE_MUX);
	case MPPT_BUS_V: return sense_get(MPPT_BUS_VSENSE);

	case CRC_ER_CNT: return errormon_get(CrcInvalid);
	case DROP_CNT:   return errormon_get(PacketTimeout);

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
	case SW_ON:  ocp_set_rails_activated(value); break;
	case CRC_ER_CNT: errormon_set(CrcInvalid, value); break;
	case DROP_CNT: errormon_set(PacketTimeout, value); break;
	}
	return;
}

