/*
 * RailOffCheck.h
 *
 *  Created on: 15 Jul 2019
 *      Author: Giles Sohi gs6g17@soton.ac.uk
 */

#ifndef PERIPHERAL_RAILOFFCHECK_H_
#define PERIPHERAL_RAILOFFCHECK_H_

//If interrupt line not brought low in 60s, restarts TOBC
inline void TOBCWatchdogService();

//Checks if the flight_mcu rail is off and switches it on
void checkTOBCOff();



#endif /* PERIPHERAL_RAILOFFCHECK_H_ */
