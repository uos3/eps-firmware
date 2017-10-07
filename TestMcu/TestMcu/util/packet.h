/*
 * packet.h
 *
 * Created: 27/10/2015 17:37:39
 *  Author: Tom
 */


#ifndef PACKET_H_
#define PACKET_H_

typedef	enum
{
	RELAY_POWER,
	SET,
	MEASURE,
	REQUEST_INT,
	RELAY_HOST = 0x80,
	RELAY_INT = 0x81,
	MEASUREMENT = 0x82,
	OCP = 0x83,
	ERROR = 0xFE,
	ACK = 0xFF
} PACKET_TYPE;

#endif /* PACKET_H_ */