/*
 *
 * @file Rails_public.h
 * @author Roberto Holmes (rh3u17@soton.ac.uk / robertoholmes@live.com)
 * @brief Public Rails component header file.
 *
 * Task ref: [EPS_SW_1.6.3]
 *
 * @version 0.1
 * @date 2021-04-10
 *
 * @copyright UOS3
 */

#ifndef COMPONENTS_RAILS_RAILS_PUBLIC_H_
#define COMPONENTS_RAILS_RAILS_PUBLIC_H_

/* -------------------------------------------------------------------------
 * INCLUDES
 * ------------------------------------------------------------------------- */

/* Standard Library */
#include <stdint.h>
#include <msp430.h>

/* Internal */
#include "components/interrupts/Interrupts_public.h"
#include "drivers/gpio/adc/Adc_public.h"
#include "drivers/gpio/mux/Mux_public.h"
#include "drivers/gpio/mux2/Mux2_public.h"

/* -------------------------------------------------------------------------
 * DEFINES
 * ------------------------------------------------------------------------- */

/* Pin number on the P1 bank */
#define RAILS_OCP1_PIN BIT2
#define RAILS_OCP2_PIN BIT1
#define RAILS_OCP3_PIN BIT0
#define RAILS_OCP4_PIN BIT5
#define RAILS_OCP5_PIN BIT4
#define RAILS_OCP6_PIN BIT3

#define RAILS_TOBC_INT_PIN BIT3

/* -------------------------------------------------------------------------
 * GLOBALS
 * ------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------
 * STRUCTS
 * ------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------
 * FUNCTIONS
 * ------------------------------------------------------------------------- */

void Rails_init();
uint8_t Rails_get_data(uint8_t *p_packet_out);
void Rails_set(uint8_t rail_num_in, uint8_t new_state_in);
uint8_t Rails_convert_rail(uint8_t rail_num_in);
uint8_t Rails_append_adc_data(uint16_t data_in, uint8_t *p_packet_out,
                              uint8_t i_inout);

#endif /* COMPONENTS_RAILS_RAILS_PUBLIC_H_ */
