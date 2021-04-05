/*
 *
 * @file InterruptManager_public.h
 * @author Roberto Holmes (rh3u17@soton.ac.uk / robertoholmes@live.com)
 * @brief Provide a level of abstraction for the interrupts.
 *
 * @version 0.1
 * @date 2021-04-05
 *
 * @copyright UOS3
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

#define INTERRUPT_MANAGER_WATCHDOG_TIMER (BIT0)
#define INTERRUPT_MANAGER_WAKE_TIMER (BIT1)
#define INTERRUPT_MANAGER_TOBC_TIMER (BIT2)


/* -------------------------------------------------------------------------
 * STRUCTS
 * ------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------
 * FUNCTIONS
 * ------------------------------------------------------------------------- */

void InterruptManager_reset_timer(uint8_t timer);

#endif /* APPLICATIONS_INTERRUPT_MANAGER_INTERRUPTMANAGER_PUBLIC_H_ */
