/*
 *
 * @file RailEditor_public.c
 * @author Roberto Holmes (rh3u17@soton.ac.uk / robertoholmes@live.com)
 * @brief Decide which rails should be turned on and off.
 *
 * @version 0.1
 * @date 2021-04-04
 *
 * @copyright UOS3
 */

#include "RailEditor_public.h"

/* Receive Rails that need to be changed in the form of bits in a byte
 * and their new state in another byte */
void RailEditor_set_rails(uint8_t rail_mask_in, uint8_t new_rail_status_in) {
    uint8_t i, rail_out, status_out;

    for (i = 0; i < 8; i++) {
        /* If the ith rail in the byte needs to be changed */
        if (rail_mask_in >> i & 1) {
            /* Set the bit corresponding to the ith rail to 1 and
             * all others to 0 */
            rail_out = rail_mask_in & (1 << i);

            /* Set the bit corresponding to the ith rail to 1 for on
             * or 0 for off and all others to 0 */
            status_out = new_rail_status_in & (1 << i);

            /* Tell the Rails component to set the new state */
            Rails_set(rail_out, status_out);
        }
    }
}

