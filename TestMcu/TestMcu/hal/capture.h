/*
 * capture.h
 *
 * Created: 02/11/2015 18:01:26
 *  Author: Tom
 */


#ifndef CAPTURE_H_
#define CAPTURE_H_

#include <stdint.h>

void capture_init();
uint32_t capture_read();
void capture_reset();

#endif /* CAPTURE_H_ */