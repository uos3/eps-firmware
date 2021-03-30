/*
 * @ingroup gpio
 *
 * @file Interrupt_public.c
 * @author Daniel Murphy (dm4g16@soton.ac.uk)
 * @brief Public source file for interrupt driver.
 *
 * @version 0.1
 * @date 2021-03-30
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

/*Internal includes */
#include "drivers/gpio/interrupt/Interrupt_public.h"
/* -------------------------------------------------------------------------
 * DEFINES
 * ------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------
 * GLOBALS
 * ------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------
 * ENUMS
 * ------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------
 * STRUCTS
 * ------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------
 * FUNCTIONS
 * ------------------------------------------------------------------------- */

void Intpin_init(void){
    /*Sets pin 1.6 as an output*/
    P1DIR |= BIT6;
    /*Sets it to output 1*/
    P1OUT |= BIT6;
}

void Intpin_set(uint8_t interruptActive){
    if(interruptActive){
        /*Sets output to 0 triggering TOBC*/
        P1OUT &= ~BIT6;
    }
    else{
        /*Sets output to 1*/
        P1OUT |= BIT6;
    }
}
