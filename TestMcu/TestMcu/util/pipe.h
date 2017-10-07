/*
 * pipe.h
 *
 * Created: 28/10/2015 15:56:24
 *  Author: Tom
 */


#ifndef PIPE_H_
#define PIPE_H_

#include <stdint.h>

typedef struct
{
	uint8_t (*length)();
	uint8_t (*pop)();
	uint8_t (*peek)(uint8_t offset);
	void (*push)(uint8_t byte);
	uint8_t (*error)();
} PIPE;

#endif /* PIPE_H_ */