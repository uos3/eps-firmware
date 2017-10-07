/*
 * error.h
 *
 * Created: 27/10/2015 18:03:55
 *  Author: Tom
 */


#ifndef ERROR_H_
#define ERROR_H_

typedef enum
{
	NONE,
	COMM_UNKNOWN_PACKET_LENGTH,
	PACKET_UNKNOWN_DEST,
	PC_UNKNOWN_PACKET,
	CIRC_PC_ERROR_NONE = 0x04,
	CIRC_DUT_ERROR_NONE = 0x08,
} ERR_CODE;

void error_throw(ERR_CODE code);


#endif /* ERROR_H_ */