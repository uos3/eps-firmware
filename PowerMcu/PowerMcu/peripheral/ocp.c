/*
 * ocp.c
 *
 *  Created on: 6 Nov 2015
 *      Author: Tom
 */

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

void processBitfield(uint8_t bits, size_t offset)
{
	if (bits)
	{
		uint8_t i;
		for (i = 0; i < 6; i++)
		{
			if (bits & (1 << i))
			{
				uint16_t *value = (uint16_t*)((uint8_t*)&railStateTransitions[i] + offset);
				(*value)++;
			}
		}
	}
}

uint16_t ocp_get_boots(uint8_t railIndex)
{
	// TODO: Map railIndex to true OCP index.
	return railStateTransitions[railIndex].boots;
}

uint16_t ocp_get_fails(uint8_t railIndex)
{
	// TODO: Map railIndex to true OCP index.
	return railStateTransitions[railIndex].overcurrents;
}

void ocp_process()
{
	static uint8_t lastOn = 0;
	static uint8_t lastActivated = 0xFF;

	uint8_t on = switches_get();
	uint8_t activated = ocp_get_rails_activated();
	uint8_t switchFlags = switches_get_flags();

	if (lastActivated != 0xFF && (on != lastOn || activated != lastActivated || switchFlags != 0))
	{
		// Bitwise process the state transitions.
		uint8_t stepActivatedOnToActivatedOff = (lastActivated & lastOn) & (activated & ~on);
		uint8_t stepActivatedOffToActivatedOn = lastActivated & ~lastOn & (activated & ~on);
		uint8_t stepDeactivatedOffToActivatedOn = (~lastActivated & ~lastOn) & (activated & on);
		uint8_t stepActivatedOnToDeactivatedOff = (lastActivated & lastOn) & (~activated & ~on);

		// Convert dual transition to two single transitions.
		const uint8_t stepDeactivatedOffToActivatedOff = (~lastActivated & ~lastOn) & (activated & ~on);
		stepDeactivatedOffToActivatedOn |= stepDeactivatedOffToActivatedOff;
		stepActivatedOnToActivatedOff   |= stepDeactivatedOffToActivatedOff;

		const uint8_t stepActivatedOffToDeactivatedOff = (lastActivated & ~lastOn) & (~activated & ~on);
		stepActivatedOffToActivatedOn   |= stepActivatedOffToDeactivatedOff;
		stepActivatedOnToDeactivatedOff |= stepActivatedOffToDeactivatedOff;

		// There is no trigger when the overcurrent event ends. Assume a interrupt triggered by an overcurrent
		// starting is equalivent to a transition from ActivatedOn to ActivatedOff.
		stepActivatedOnToActivatedOff   |= (switchFlags & activated & lastActivated);

		// Count the transitions.
		// TODO: Fix overcurrent event detection
		processBitfield(stepDeactivatedOffToActivatedOn, offsetof(stateTransitions_t, boots));
		processBitfield(stepActivatedOnToDeactivatedOff, offsetof(stateTransitions_t, shutdowns));
		processBitfield(stepActivatedOnToActivatedOff | (switchFlags & activated & lastActivated), offsetof(stateTransitions_t, overcurrents));
		processBitfield(stepActivatedOffToActivatedOn, offsetof(stateTransitions_t, overcurrentEnds));

		// Fire an interrupt for the overcurrent events.
		status_set_bits((status_t)stepActivatedOnToActivatedOff);
	}

	lastOn = on;
	lastActivated = activated;
}

void ocp_activate_rail(uint8_t ocp_rail)
{
    uint8_t switchDiff = 0x3F & ~switches_get_direction();
    switchDiff |= ocp_rail;
    switches_set_direction(0x3F & ~switchDiff);

    uint8_t temp = switches_get_direction();

    ocp_process();
}

void ocp_deactivate_rail(uint8_t ocp_rail)
{
    uint8_t switchDiff = switches_get_direction();
    switchDiff |= ocp_rail;
    switches_set_direction(switchDiff);

    uint8_t temp = switches_get_direction();

    ocp_process();
}

uint8_t ocp_get_rails_activated()
{
	return ~switches_get_direction() & 0x3F;
}
