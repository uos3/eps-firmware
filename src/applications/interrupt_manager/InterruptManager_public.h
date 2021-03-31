/*
 * Timer_public.h
 *
 *  Created on: 15 Mar 2021
 *      Author: Roberto
 */

#ifndef APPLICATIONS_INTERRUPT_MANAGER_INTERRUPTMANAGER_PUBLIC_H_
#define APPLICATIONS_INTERRUPT_MANAGER_INTERRUPTMANAGER_PUBLIC_H_

/* -------------------------------------------------------------------------
 * INCLUDES
 * ------------------------------------------------------------------------- */
/* Standard library includes */
#include <msp430.h>
#include <stdint.h>

/* -------------------------------------------------------------------------
 * GLOBALS
 * ------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------
 * DEFINES
 * ------------------------------------------------------------------------- */

#define INTERRUPT_MANAGER_TOBC_TIMER (0)
#define INTERRUPT_MANAGER_TIMER1_A2 (1)

/* -------------------------------------------------------------------------
 * STRUCTS
 * ------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------
 * FUNCTIONS
 * ------------------------------------------------------------------------- */

int InterruptManager_reset_timer(uint8_t timer);

#endif /* APPLICATIONS_INTERRUPT_MANAGER_INTERRUPTMANAGER_PUBLIC_H_ */
