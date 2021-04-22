#include <stdio.h>
#include <stdint.h>
#include <msp430.h>

#include "drivers/i2c/I2c_public.h"


int main(void) {
    /*Stop watchdog*/
    WDTCTL = WDTPW + WDTHOLD; // Stop WDT
    /*Calibrate clock for 1MHz*/
    DCOCTL = 0;
    BCSCTL1 = CALBC1_1MHZ;
    DCOCTL = CALDCO_1MHZ;
    uint8_t test[1];
    test[0] = 0x45;
    I2c_master_init();
    I2c_master_write(0x09, 1, test);
    while(1);
}
