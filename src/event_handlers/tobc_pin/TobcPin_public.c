/*
 * Tobc_public.c
 *
 *  Created on: 29 Mar 2021
 *      Author: Roberto
 */

#include "TobcPin_public.h"

int TobcPin_event() {
    return InterruptManager_reset_timer(INTERRUPT_MANAGER_TOBC_TIMER);
}
