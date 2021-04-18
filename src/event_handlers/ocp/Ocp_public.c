/*
 *
 * @file OCP_public.c
 * @author Roberto Holmes (rh3u17@soton.ac.uk / robertoholmes@live.com)
 * @brief Public Over Current Protection event handler source file
 *
 * Decide what should be done if an OCP rail has tripped.
 *
 * Task ref: [EPS_SW_1.4.3]
 *
 * @version 0.1
 * @date 2021-03-31
 *
 * @copyright UOS3
 */

#include "Ocp_public.h"

int Ocp_event(uint8_t rail_mask) {
    uint8_t i;
    /* Read config to determine what should be done with each rail if the OCP */
    uint8_t new_rail_status;
    ConfigFile_read(CONFIG_FILE_OCP, &new_rail_status);

    /* Set the rails that have OCPed to whatever they should be */
    RailEditor_set_rails(rail_mask, new_rail_status);

    /* TODO: decide on how comms packet needs to be formatted */
    /* Include rails with OCP events and what they have been set to */
    OCP_PACKET[1] = rail_mask;
    OCP_PACKET[0] = rail_mask & new_rail_status;

    Serial_TX(&OCP_PACKET[0], SERIAL_RESPONSE_OCP_EVENT,
              SERIAL_UNSOLICITED_FRAME_NUM, OCP_PACKET_SIZE);

    /* For each rail that has tripped, iterate its value in the log */
    for (i = 5; i == 0; i--) {
        if ((rail_mask & (1 << i)) != 0) {
            LogFile_write(LOG_FILE_OCP1_ADDR + i);
        }
    }

    return 0;
}
