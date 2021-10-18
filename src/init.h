/*
 *
 * @file init.h
 * @author Roberto Holmes (rh3u17@soton.ac.uk / robertoholmes@live.com)
 * @brief Header file for initialisation functions.
 *
 * @version 0.1
 * @date 2021-03-29
 *
 * @copyright UOS3
 */

#ifndef INIT_H_
#define INIT_H_

/* -------------------------------------------------------------------------
 * INCLUDES
 * ------------------------------------------------------------------------- */
/* Standard library includes */
#include <msp430.h>

/* internal */
#include "applications/interrupt_manager/InterruptManager_public.h"
#include "applications/log_file/LogFile_public.h"
#include "applications/battery_comms/BatteryComms_public.h"

#include "components/rails/Rails_public.h"
#include "components/serial/Serial_public.h"
#include "components/interrupts/Interrupts_public.h"
#include "components/battery/Battery_public.h"

#include "drivers/uart/Uart_public.h"
#include "drivers/gpio/mux/Mux_public.h"
#include "drivers/gpio/mux2/Mux2_public.h"
#include "drivers/gpio/adc/Adc_public.h"
#include "drivers/flash/Flash_public.h"
#include "drivers/i2c/I2c_public.h" //Needed only for battery RX/TX operation

/* -------------------------------------------------------------------------
 * GLOBALS
 * ------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------
 * DEFINES
 * ------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------
 * STRUCTS
 * ------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------
 * FUNCTIONS
 * ------------------------------------------------------------------------- */

void init_msp430();
void init_drivers();
void init_components();
void init_applications();
void init_startup();
void init_interrupts();
void I2c_master_init(uint8_t slaveaddress_in);
#endif /* INIT_H_ */
