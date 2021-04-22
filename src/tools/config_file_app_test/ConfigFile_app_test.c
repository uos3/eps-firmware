/*
 *
 * @file ConfigFile_app_test.c
 * @author Roberto Holmes (rh3u17@soton.ac.uk / robertoholmes@live.com)
 * @brief Test for the Config File and all relevant lower level modules.
 *
 * Task ref: [EPS_SW_1.5.6]
 *
 * @version 0.1
 * @date 2021-04-22
 *
 * @copyright UOS3
 */

#include "ConfigFile_app_test.h"

uint8_t DUMMY_CONFIG[3];


int main(void){
    uint8_t config_output;
    volatile uint8_t return_value;

    /* Add some data to write to config */
    DUMMY_CONFIG[0] = 0x46;
    DUMMY_CONFIG[1] = 0xFF;
    DUMMY_CONFIG[2] = 0xFF;

    /* write the data and geta ny errors */
    return_value = ConfigFile_write(DUMMY_CONFIG);

    /* Do nothing so that we can pause and debug */
    __no_operation();

    return_value = ConfigFile_read_8bit(CONFIG_FILE_RESET_RAIL_AFTER_OCP, &config_output);

    __no_operation();

    if(config_output == DUMMY_CONFIG[0]){
        /* Test passed */
        return 0;
    }
    else{
        /* Test failed */
        return 1;
    }
}


