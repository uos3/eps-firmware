/*
 * @ingroup gpio
 *
 * @file Mux_public.c
 * @author Daniel Murphy (dm4g16@soton.ac.uk)
 * @brief Public Mux driver
 *
 * Implements Mux driver for the MSP4302132
 *
 * @version 0.1
 * @date 2021-03-29
 *
 * @copyright UOS3
 */
/* -------------------------------------------------------------------------
 * INCLUDES
 * ------------------------------------------------------------------------- */

/* Standard library includes */
#include <stdio.h>
#include <stdint.h>
#include <msp430.h>
#include "drivers/gpio/mux/Mux_public.h"
/* -------------------------------------------------------------------------
 * DEFINES
 * ------------------------------------------------------------------------- */
/* @brief Deals with P2.4 not being used for MUX */
#define MUX_PINS (0x2F)
/* @brief MUX channel selected was outside of the allowed range*/
#define MUX_CHANNEL_OUTSIDE_RANGE (0x01)
/* -------------------------------------------------------------------------
 * FUNCTIONS
 * ------------------------------------------------------------------------- */

void Mux_init(void){
    /* Sets Pins P2.0, P2.1, P2.2, P2.3 and P2.5 as outputs
     * P2.4 is not used in the MUX
     */
    P2DIR |= MUX_PINS;
}

uint8_t Mux_select(uint8_t channel){
    /* Takes input channel number and remaps for the analog pins
     * P2.0 - A4, P2.1 - A3, P2.2 - A2, P2.3 - A1, P2.5 - A0
     * Then sets appropriate logic to select the channel by turning all pins off
     * then setting the appropriate pins on
     */
    if(channel > 31){
        return MUX_CHANNEL_OUTSIDE_RANGE;
    }
    else{
        channel = ((channel & 0x01) << 5) | ((channel & 0x02) << 2) | ((channel & 0x04) << 0) | ((channel & 0x08) >> 2) | ((channel & 0x10) >> 4);
        P2OUT = (P2OUT & ~MUX_PINS) | channel;
        return 0;
    }
}

