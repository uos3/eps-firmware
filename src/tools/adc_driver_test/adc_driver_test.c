

#include "main.h"

int main(void) {
    /*Disable Watchdog*/
    WDTCTL = WDTPW + WDTHOLD;
    /*Calibrate clock*/
    DCOCTL = 0;
    BCSCTL1 = CALBC1_1MHZ;
    DCOCTL = CALDCO_1MHZ;
    uint16_t test;
    /*Initialise*/
    Adc_init();
    Mux_init();
    /*Select MUX channel 27 channels are 0-31, not 1-32 as shown in the schematic*/
    Mux_select(27);
    test = Adc_convert(INCH_7);
    while(1);
}

