#include "msp430x21x2.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "drivers/i2c/I2c_public.h"

uint8_t TxData[1];
uint8_t RxData[1];

void main(void)
{
  WDTCTL = WDTPW + WDTHOLD;
  TxData[0] = 0x01;// Stop WDT
  I2c_master_init(0x8);
  I2c_master_write(0x8, 1, TxData);
  __bis_SR_register(CPUOFF + GIE); // Enable TX interrupt
  I2c_master_read(0x8, 1, RxData);



  while (1)
  {           // I2C TX, start condition       // Enter LPM0 w/ interrupts
                                            // Remain in LPM0 until all data
                                            // is TX'd
  }
}
