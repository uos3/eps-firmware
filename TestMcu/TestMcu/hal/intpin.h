/*
 * intpin.h
 *
 * Created: 28/10/2015 16:28:14
 *  Author: Tom
 */


#ifndef INTPIN_H_
#define INTPIN_H_

void intpin_init();
uint8_t intpin_get();
uint8_t intpin_get_changed();
void intpin_clear_changed();

#endif /* INTPIN_H_ */