/*
 * @ingroup gpio
 *
 * @file Mux2_public.c
 * @author Daniel Murphy (dm4g16@soton.ac.uk)
 * @brief Public file for MUX2 driver. This second MUX driver was needed as the second multiplexer is a different chip
 * to the original and therefore requires different operation.
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
#include "drivers/gpio/mux2/Mux2_public.h"

/* -------------------------------------------------------------------------
 * DEFINES
 * ------------------------------------------------------------------------- */
/* @brief MUX2 channel selected was outside of the allowed range*/
#define MUX2_CHANNEL_OUTSIDE_RANGE (0x01)


/* -------------------------------------------------------------------------
 * FUNCTIONS
 * ------------------------------------------------------------------------- */

void Mux2_init(void){
    /* Sets Pins P3.0 and P2.4 as outputs
     */
    P2DIR |= BIT4;
    P3DIR |= BIT0;

}

uint8_t Mux2_select(uint8_t channel){
    /* Takes input channel number and remaps for the analog pins
     * P3.0 - A0, P2.4 - A1
     * Then sets appropriate logic to select the channel by turning all pins off
     * then setting the appropriate pins on
     */
    uint8_t A0;
    uint8_t A1;
    if(channel > 3){
        return MUX2_CHANNEL_OUTSIDE_RANGE;
    }
    else{
        A0 = ((channel & 0x01));
        A1 = ((channel & 0x02) << 3);
        P3OUT = (P3OUT & ~BIT0) | A0;
        P2OUT = (P2OUT & ~BIT4) | A1;
        return 0;
    }
}

