/*
 * dac.c
 *
 * Created: 02/11/2015 17:04:58
 *  Author: Tom
 */

#include <hal/spi.h>
#include "ocp.h"

void dac_set(uint8_t index, uint8_t value)
{
	spi_ss(1);
	spi_tx(index);
	spi_tx(value);
	spi_ss(0);
	ocp_reset();
}
