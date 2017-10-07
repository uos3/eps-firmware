/*
 * spi.c
 *
 * Created: 02/11/2015 16:35:38
 *  Author: Tom
 */

#include <stdint.h>
#include <avr/io.h>

#define SPI_ENABLE (1 << SPE)
#define SPI_MSB_FIRST (0 << DORD)
#define SPI_MASTER (1 << MSTR)
#define SPI_CLK_POLARITY_HIGH (1 << CPOL)
#define SPI_CLK_PHASE_TRAILING (1 << CPHA)

void spi_init()
{
	// PB4 - nSS
	// PB5 - MOSI
	// PB6 - MISO
	// PB7 - SCK
	PORTB |= 1<<PB4;
	DDRB = (DDRB & 0x0F) | (1<<PB4) | (1<<PB5) | (1<<PB7);
	SPCR = SPI_ENABLE
	     | SPI_MSB_FIRST
	     | SPI_MASTER
		 | SPI_CLK_POLARITY_HIGH
		 | SPI_CLK_PHASE_TRAILING;
}

void spi_tx(uint8_t value)
{
	SPDR = value;
	while(!(SPSR & (1<<SPIF)));
}

void spi_ss(uint8_t slave)
{
	switch (slave)
	{
		case 0:
			PORTB |= (1<<PB4);
			break;
		case 1:
			PORTB &= ~(1<<PB4);
			break;
	}
}
