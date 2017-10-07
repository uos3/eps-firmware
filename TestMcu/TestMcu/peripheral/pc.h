/*
 * pc.h
 *
 * Created: 28/10/2015 15:39:09
 *  Author: Tom
 */


#ifndef PC_H_
#define PC_H_

#include <stdint.h>
#include <util/pipe.h>
#include "error.h"

void pc_process();
void pc_tx_relay_host(PIPE *src, uint8_t len);
void pc_tx_error(ERR_CODE code);
void pc_tx_relay_int(uint8_t status);
void pc_tx_data(uint16_t *data);
void pc_tx_ocp(uint32_t ticks);

#endif /* PC_H_ */