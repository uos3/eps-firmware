/*
 *
 * @file RailEditor_public.h
 * @author Roberto Holmes (rh3u17@soton.ac.uk / robertoholmes@live.com)
 * @brief Decide which rails should be turned on and off.
 *
 * @version 0.1
 * @date 2021-04-04
 *
 * @copyright UOS3
 */

#ifndef APPLICATIONS_RAIL_EDITOR_RAILEDITOR_PUBLIC_H_
#define APPLICATIONS_RAIL_EDITOR_RAILEDITOR_PUBLIC_H_

/* -------------------------------------------------------------------------
 * INCLUDES
 * ------------------------------------------------------------------------- */

/* Standard Library */
#include <stdint.h>
#include <msp430.h>

/* Internal */
#include "components/rails/Rails_public.h"

/* -------------------------------------------------------------------------
 * GLOBALS
 * ------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------
 * DEFINES
 * ------------------------------------------------------------------------- */

/* Pin number on the P1 bank */
#define OCP1_PIN BIT2
#define OCP2_PIN BIT1
#define OCP3_PIN BIT0
#define OCP4_PIN BIT5
#define OCP5_PIN BIT4
#define OCP6_PIN BIT3
#define TOBC_PIN BIT3

#define RAIL_EDITOR_TOBC_RAIL OCP4_PIN

/* -------------------------------------------------------------------------
 * STRUCTS
 * ------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------
 * FUNCTIONS
 * ------------------------------------------------------------------------- */
void RailEditor_set_rails(uint8_t rail_mask_in, uint8_t new_rail_status_in);

#endif /* APPLICATIONS_RAIL_EDITOR_RAILEDITOR_PUBLIC_H_ */
