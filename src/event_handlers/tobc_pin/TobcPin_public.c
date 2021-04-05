/*
 * Tobc_public.c
 *
 *  Created on: 29 Mar 2021
 *      Author: Roberto
 */

#include "TobcPin_public.h"

void TobcPin_event() {
    InterruptManager_reset_timer(INTERRUPT_MANAGER_TOBC_TIMER);
}
