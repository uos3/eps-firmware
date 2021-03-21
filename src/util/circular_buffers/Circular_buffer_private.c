/**
 * @ingroup circular_buffer
 * @file Circular_buffer_private.c
 * @author Daniel Murphy (dm4g16@soton.ac.uk)
 * @brief Public circular_buffer
 *
 * Implements private functions for circular buffers used in the UART driver
 *
 * @version 0.1
 * @date 2021-03-15
 *
 * @copyright UOS3
 */


/* -------------------------------------------------------------------------
 * INCLUDES
 * ------------------------------------------------------------------------- */

/* Standard library includes */
#include <stdio.h>
#include <stdint.h>

/*Internal includes*/
#include "util/circular_buffers/Circular_buffer_public.h"

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
/**
 *  File header comment
 */

/* -------------------------------------------------------------------------
 * INCLUDES
 * ------------------------------------------------------------------------- */

/* Standard library includes */
#include <stdio.h>

/* TivaWare includes */
#include "inc/tm4c123gh6pm.h"

/* Internal includes */
#include "driver/spi/spi_public.h"

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
 uint8_t Circular_buffer_length(circ *p_c){
     /* Returns the current length of the buffers content*/
     uint8_t end = p_c->end;
     uint8_t start = p_c->start;
     if (end < start){
         return CIRC_MAX - start + end;
     }
     else
         return end - start;
 }

