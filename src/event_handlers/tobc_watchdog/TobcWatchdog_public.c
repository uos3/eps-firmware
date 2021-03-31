/*
 *
 * @file TobcWatchdog_public.c
 * @author Roberto Holmes (rh3u17@soton.ac.uk / robertoholmes@live.com)
 * @brief Deal with an interrupt cause by the timer assigned to the TOBC Watchdog.
 *
 * @version 0.1
 * @date 2021-03-31
 *
 * @copyright UOS3
 */

#include "TobcWatchdog_public.h"

int TobcWatchdog_event(){
    RailEditor_set_rails(RAIL_EDITOR_TOBC_RAIL, 0);
    RailEditor_set_rails(RAIL_EDITOR_TOBC_RAIL, RAIL_EDITOR_TOBC_RAIL);
    InterruptManager_reset_timer(INTERRUPT_MANAGER_TOBC_TIMER);
    LogFile_write(LOG_FILE_TOBC_TIMER_COUNT_ADDR);
    return 0;
}


