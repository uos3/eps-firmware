/*
 * spi.h
 *
 * Created: 02/11/2015 16:35:46
 *  Author: Tom
 */


#ifndef SPI_H_
#define SPI_H_

#include <stdint.h>

void spi_init();
void spi_tx(uint8_t value);

// Slave select
//   slave = 0 for no slave, 1-n for other slaves.
void spi_ss(uint8_t slave);

#endif /* SPI_H_ */