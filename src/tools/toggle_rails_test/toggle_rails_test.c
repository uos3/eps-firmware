/*
 *
 * @file toggle_rail_test.c
 * @author Roberto Holmes (rh3u17@soton.ac.uk / robertoholmes@live.com)
 * @brief Test the ability of the Rails component to be able to change
 * the state of OCP rails.
 *
 * Task ref: [EPS_SW_1.6.3]
 *
 * @version 0.1
 * @date 2021-04-22
 *
 * @copyright UOS3
 */

#include "toggle_rails_test.h"

/* This is designed to be run using the debug mode and
 * stepping through the code.
 */
int main(void){
    Rails_set(BIT0, 0);
    Rails_set(BIT1, 0);
    Rails_set(BIT2, 0);
    Rails_set(BIT3, 0);
    Rails_set(BIT4, 0);
    Rails_set(BIT5, 0);
    /* Set breakpoint here and measure the voltages at the test points
     * (All rails should be off, BIT 3 controls the EPS MCU so will need
     * to be removed if this is turning off the processor.) */
    __no_operation();
    Rails_set(BIT0, 1);
    Rails_set(BIT1, 1);
    Rails_set(BIT2, 1);
    Rails_set(BIT3, 1);
    Rails_set(BIT4, 1);
    Rails_set(BIT5, 1);
    /* Another breakpoint and measurement (All rails on) */
    __no_operation();
    Rails_set(BIT0, 0);
    Rails_set(BIT1, 0);
    Rails_set(BIT2, 0);
    Rails_set(BIT3, 0);
    Rails_set(BIT4, 0);
    Rails_set(BIT5, 0);
    /* Another breakpoint and measurement (All rails off) */
    __no_operation();
}


