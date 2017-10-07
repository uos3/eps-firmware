/*
 * dut.h
 *
 * Created: 28/10/2015 15:44:47
 *  Author: Tom
 */


#ifndef DUT_H_
#define DUT_H_

#include <stdint.h>
#include <util/pipe.h>

void dut_process();
void dut_tx(PIPE *src, uint8_t len);

#endif /* DUT_H_ */