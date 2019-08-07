/*
 * errormon.c
 *
 *  Created on: 31 Oct 2015
 *      Author: Tom
 */

//Error monitoring using cyclic redundancy checks (CRC)

#include <stdint.h>
#include "status.h"
#include "errormon.h"


static uint16_t errors[2];

void errormon_set(ERROR_t error, uint16_t value)
{
	if (error > PacketTimeout)     //Making sure the error type is valid
		return;

	errors[error] = value;
}

uint16_t errormon_get(ERROR_t error)    //Gets either the number of packet timeouts (ERROR_LAST) or the number of incorrect CRC's
{
	if (error > PacketTimeout)     //Making sure the error type is valid
		return 0xFFFF;

	return errors[error];
}

void errormon_increment(ERROR_t error)
{
	if (error > PacketTimeout)     //Making sure the error type is valid
		return;

	//      As the TOBC comms always end with a comm from the EPS, these are not needed for the TOBC to learn of a failed communication
    if (error == CrcInvalid)
    {
        status_set_bits(CRCBit);
    }
    if (error == PacketTimeout)
    {
        status_set_bits(PTBit);
    }


	errormon_set(error, errormon_get(error) + 1);
}

