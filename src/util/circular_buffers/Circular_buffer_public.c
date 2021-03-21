/*
 * @ingroup circular_buffer
 * @file Circular_buffer_public.c
 * @author Daniel Murphy (dm4g16@soton.ac.uk)
 * @brief Public circular_buffer
 *
 * Implements public functions for circular buffers used in the UART driver
 *
 * @version 0.1
 * @date 2021-03-15
 *
 * @copyright UOS3
 */

/* -------------------------------------------------------------------------
 * INCLUDES
 * ------------------------------------------------------------------------- */
#include "util/circular_buffers/Circular_buffer_public.h"
#include "util/circular_buffers/Circular_buffer_private.h"


/* Standard library includes */
#include <stdio.h>

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

 void Circular_buffer_init(circ *p_c){
     /* Sets the values of start and end to 0 */
    p_c->start = 0;
    p_c->end = 0;
}

 uint8_t Circular_buffer_push(circ *p_c, uint8_t value){
     /* Checks if the queue is empty before pushing the data given in the parameter
      * value to the buffer*/
     if (Circular_buffer_length(p_c) < CIRC_MAX - 1){
         uint8_t end = p_c->end;
         p_c->buffer[end] = value;
         p_c->end = (end + 1) % CIRC_MAX;
         return 1;
     }
     else
         /*Error: Queue Full*/
         return 0;
 }

 uint8_t Circular_buffer_pop(circ *p_c){
     /*This function checks if there is anything in the buffer, if the buffer is not
      * empty then it returns the data in the buffer*/
     uint8_t start = p_c->start;
     if (start != p_c->end){
         uint8_t result = p_c->buffer[start];
         p_c->start = (start + 1) % CIRC_MAX;
         return result;
     }
     else
         /*Error: Queue Empty */
         return 0xFF;
 }

