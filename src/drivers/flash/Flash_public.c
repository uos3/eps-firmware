/*
 * @ingroup flash
 *
 * @file Flash_public.c
 * @author Daniel Murphy (dm4g16@soton.ac.uk)
 * @brief Public Flash driver source file
 *
 * Implements Flash driver for the MSP4302132
 *
 * @version 0.1
 * @date 2021-04-10
 *
 * @copyright UOS3
 */

/* -------------------------------------------------------------------------
 * INCLUDES
 * ------------------------------------------------------------------------- */

#include "Flash_public.h"

/* -------------------------------------------------------------------------
 * FUNCTIONS
 * ------------------------------------------------------------------------- */
uint8_t Flash_init(void) {
    /*@brief initialises flash, sets timing generator to 400kHz (8MHz / 20)*/
    FCTL2 = FWKEY + FSSEL_2 + FN4 + FN1 + FN0;
    return 0;
}

uint8_t Flash_read(uint8_t address, uint8_t bytecount_in, uint8_t *p_data_out) {
    /*@brief sets the address and reads the data from the flash*/
    int i;
    /* Add an offset to the address to point at the information memory */
    uint16_t *p_Flash = (uint16_t *) (0x1000 | ((uint16_t)address));
//    uint8_t *p_Flash = (uint8_t *) address;
    for(i = 0; i < bytecount_in; i ++) {
        *p_data_out = *p_Flash;
        p_Flash++;
        p_data_out++;
    }
    return 0;
}

uint8_t Flash_write(uint8_t address, uint8_t bytecount_in, uint8_t *p_data_in) {
    /*@brief writes to the flash by first erasing, and then writing the data from p_data_in*/
    int i;
    uint16_t *p_Flash = (uint16_t *) (0x1000 | ((uint16_t)address));
//    uint8_t *p_Flash = (uint8_t *) address;
    FCTL3 = FWKEY;
    FCTL1 = FWKEY + ERASE;
    *p_Flash = 0;
    FCTL1 = FWKEY + WRT;
    for(i = 0; i <bytecount_in; i ++) {
        *p_Flash = *p_data_in;
        p_Flash++;
        p_data_in++;
    }
    return 0;
}


