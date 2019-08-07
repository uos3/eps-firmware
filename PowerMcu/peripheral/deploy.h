/*
 * deploy.h
 *
 *  Created on: 8 Jan 2016
 *      Author: Tom
 *      Commented by: Tom Piga
 */

#ifndef PERIPHERAL_DEPLOY_H_
#define PERIPHERAL_DEPLOY_H_

#include <stdint.h>

/* This is a simple delay which waits until the OBC on the TOBC board turns
 * on the remaining 5 rails. This includes performing checks, and is hence
 * given a moderate time to execute */
void deploy_wait_to_activate(uint16_t seconds);

#endif /* PERIPHERAL_DEPLOY_H_ */
