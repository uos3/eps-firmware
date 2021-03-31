/*
 *
 * @file Serial_public.c
 * @author Roberto Holmes (rh3u17@soton.ac.uk / robertoholmes@live.com)
 * @brief Decide what should be done if a serial RX interrupt has occurred.
 *
 * Task ref: [EPS_SW_1.4.1]
 *
 * @version 0.1
 * @date 2021-03-31
 *
 * @copyright UOS3
 */

#include <event_handlers/serial_event/SerialEvent_public.h>


int SerialEvent_event(){
    return SerialComms_process();
}
