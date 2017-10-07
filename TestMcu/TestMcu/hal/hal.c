/*
 * hal.c
 *
 * Created: 27/10/2015 17:50:30
 *  Author: Tom
 */

#include "uart.h"
#include "intpin.h"
#include "adc.h"
#include "spi.h"
#include "capture.h"

void hal_init()
{
	uart_init();
	intpin_init();
	adc_init();
	spi_init();
	capture_init();
}