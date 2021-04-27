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


void SerialEvent_event(uint8_t event_type_in){
    switch(event_type_in){
    case SERIAL_EVENT_PREPARE_PACKET:
        SerialComms_prepare_packet();
        break;
    case SERIAL_EVENT_CONTINUE:
        SerialComms_continue(SERIAL_COMMS_VALID_CONTINUE);
        break;
    case SERIAL_EVENT_INVALID_CONTINUE:
        SerialComms_continue(SERIAL_COMMS_INVALID_CONTINUE);
        break;
    }
}
