/*
 * ocp.c
 *
 *  Created on: 6 Nov 2015
 *      Author: Tom
 */

//Over current protection is done by hardware but instances of its triggering need to be recorded... plus this deals with TOBC commanded resets of devices

#include "hal/switches.h"
#include "status.h"
#include "ocp.h"
#include "stddef.h"

typedef struct {
	uint16_t boots;
	uint16_t shutdowns;
	uint16_t overcurrents;
	uint16_t overcurrentEnds;
} stateTransitions_t;

static stateTransitions_t railStateTransitions[6];

void processBitfield(uint8_t bits, size_t offset)   //Used to update values of railStateTransitions
{
	if (bits)
	{
		uint8_t i;
		for (i = 0; i < 6; i++)
		{
			if (bits & (1 << i))
			{
				uint16_t *value = (uint16_t*)((uint8_t*)&railStateTransitions[i] + offset); //Should the first line have ++ without creating a new variable and is the middle cast to a pointer to a byte needed
				(*value)++;
			}
		}
	}
}

uint16_t ocp_get_boots(uint8_t railIndex)
{
	//No mapping needed
	return railStateTransitions[railIndex].boots;
}

uint16_t ocp_get_fails(uint8_t railIndex)
{
	//No mapping needed
	return railStateTransitions[railIndex].overcurrents;
}

void ocp_process() //If called after a change in state of a power rail calculates the predicted change in state and records it
{
	static uint8_t lastOn = 0;
	static uint8_t lastActivated = 0;
	static uint8_t on = 0;
	static uint8_t switchFlags = 0;
	uint8_t activated = ocp_get_rails_activated();
	if (activated)  {                       //Input states not reliable when activated as pins in output mode, assumes unchanged instead
	    on = switches_get();        //Meaning ocp not tripped provided the pin is activated
	    switchFlags = switches_get_flags();     //1 when OCP tripped, useful when rail tripped and reset since last call of this routine
	}   else    {
	    switches_get_flags();       //Clearing flags if they are not reliable
	}


	if (lastActivated != 0xFF && (on != lastOn || activated != lastActivated || switchFlags != 0))
	{
		// Count the transitions.
		processBitfield(activated & ~lastActivated, offsetof(stateTransitions_t, boots));       //Boots defined as any activation since last called
		processBitfield(~activated & lastActivated, offsetof(stateTransitions_t, shutdowns));   //Any deactivation since last called
		processBitfield( (~on & lastOn)|switchFlags, offsetof(stateTransitions_t, overcurrents));     //
		processBitfield( (on & ~lastOn)|(switchFlags & on), offsetof(stateTransitions_t, overcurrentEnds));

		// Fire an interrupt for the overcurrent events.
		status_set_bits((uint8_t)(((~on & lastOn)|switchFlags) & 0x3F));
	}

	lastOn = on;
	lastActivated = activated;
}

void ocp_toggle_rail(uint8_t ocp_rail)  {       //Toggles the rail which OCP rail holds a number 1 in the place of, through use of XOR
    uint8_t switchState = switches_get_direction();
    switchState ^= ocp_rail;
    switches_set_direction(0x3F & switchState);     //The first two pins aren't used for rails so ensures they can't be changed
}//Switching between input and output used instead of outputting a signal as this changes the impedance which allows the OCP to take priority if a rail needs to be deactivated


void ocp_deactivate_all_rails() {
    switches_set_direction(0x3F);     //Setting all rails to 1 (output direction) (low impedance)
}


uint8_t ocp_get_rails_activated()
{
	return ~switches_get_direction() & 0x3F;
}
