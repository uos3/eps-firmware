/*
 * uart.c
 *
 *  Created on: 5 Sept 2018
 *      Author: Ossicode
 *      Ported by: Tom Piga
 *      Note from Tom: don't worry about the to-do(s), they shouldn't be
 *      necessary for basic I2C functionality.
 */

#include "i2c.h"



// Slave
#define I2C_SLAVE_BUFFER_SIZE 64

static volatile uint16_t slaveIndex;
static volatile uint16_t defaultSlaveIndex;

static volatile uint8_t beaconPacketAddress;

static uint8_t i2cTxDone; // 0 = in transfer , 1 = done
static uint8_t i2cRxDone; // 0 = in transfer , 1 = done


void i2c_portSetup(void)
{
    // For MSP430F2132, P3.1 = SDA, P3.2 = SCL
    P3SEL |= 0x06;
}

void i2c_enable(void)
{

}

void i2c_disable(void)
{

}

void i2c_reset(void)
{
    UCB0CTL1 |= UCSWRST;
    UCB0CTL1 &= ~UCSWRST;
}

void i2c_enableRXInterrupt(void)
{
    IE2 |= UCB0RXIE;
}

void i2c_disableRXInterrupt(void)
{
    IE2 &= ~UCB0RXIE;
}

void i2c_enableTXInterrupt(void)
{
    IE2 |= UCB0TXIE;
}

void i2c_disableTXInterrupt(void)
{
    IE2 &= ~UCB0TXIE;
}

void i2c_enableUCB0Interrupt(uint8_t interruptSelect)
{
    UCB0I2CIE |= interruptSelect;
}

void i2c_disableUCB0Interrupt(uint8_t interruptSelect)
{
    UCB0I2CIE &= ~interruptSelect;
}

void i2c_setTxDoneFlag(void)
{
    i2cTxDone = 1;
}

void i2c_clearTxDoneFlag(void)
{
    i2cTxDone = 0;
}

uint8_t i2c_getTxDoneFlag(void)
{
    return i2cTxDone;
}

void i2c_setRxDoneFlag(void)
{
    i2cRxDone = 1;
}

void i2c_clearRxDoneFlag(void)
{
    i2cRxDone = 0;
}

uint8_t i2c_getRxDoneFlag(void)
{
    return i2cRxDone;
}

// call this only when you need need master
void i2c_masterInit(uint8_t selctClockSource, uint16_t preScalerValue ,uint8_t modeSelect)
{
//    ASSERT((selctClockSource == I2C_CLOCKSOURCE_ACLK)||(selctClockSource == I2C_CLOCKSOURCE_SMCLK));
//    ASSERT((preScalerValue>=4) && (preScalerValue<=0xFFFF));
//    ASSERT((modeSelect == I2C_TRANSMIT_MODE)||(modeSelect == I2C_RECEIVE_MODE));

    // reset I2C
    UCB0CTL1 = UCSWRST;
    // reset and set UCB0CTL0 for I2C mode
    // UCA10 = 0 own address 7 bit          //These default to 0 anyway
    // UCSLA10 = 0 slave address 7bit
    // UCMM = 0 no multi-master
    UCB0CTL0 = UCMST + UCMODE_3 + UCSYNC;
    // reset clock source while UCSWRST = 1
    UCB0CTL1 = selctClockSource + modeSelect + UCSWRST;
    // fSCL = selctClockSource / preScalerValue
    // for single master mode, minimum preScalerValue = 4
    UCB0BR0 = preScalerValue & 0xFF;
    UCB0BR1 = (preScalerValue >> 8) & 0xFF;     //100kHz = 80/fSMCLK

    UCB0CTL1 &= ~UCSWRST;

}

void i2c_masterRead(uint8_t slaveAddress, uint8_t byteCount, uint8_t *data)
{
//    ASSERT((byteCount >=1)&&(byteCount<=I2C_MASTER_RX_BUFFER_SIZE));
    masterRxData = data;
    // slaveAddress is right justified. bit 6 is MSB for 7 bit address
    UCB0I2CSA = slaveAddress;
    UCB0CTL1 &= ~UCTR;                            //Receive mode


    if (byteCount == 1)
    {
        masterRxIndex = byteCount ;
        __disable_interrupt();
        UCB0CTL1 |= UCTXSTT;                      // I2C start condition
        while (UCB0CTL1 & UCTXSTT);               // Start condition sent?
        UCB0CTL1 |= UCTXSTP;                      // I2C stop condition
        __enable_interrupt();
    }
    else if (byteCount > 1)
    {
        IE2 |= UCB0RXIE;
        masterRxIndex = byteCount ;
        UCB0CTL1 |= UCTXSTT;                    // I2C start condition
    }
}

void i2c_masterWrite(uint8_t slaveAddress, uint8_t byteCount, uint8_t *data)
{
    //ASSERT((byteCount >=1)&&(byteCount<=I2C_MASTER_TX_BUFFER_SIZE));
    masterTxData = data;
    masterTxIndex = byteCount;
    // slaveAddress is right justified. bit 6 is MSB for 7 bit address
    UCB0CTL1 |= UCTR;                       //Transmit mode
    UCB0I2CSA = slaveAddress;
    IE2 |= UCB0TXIE;
    UCB0CTL1 |=  UCTXSTT;                   // I2C TX, start condition
}



void i2c_busRecovery(void)
{
    volatile uint8_t i;
    P3SEL &= ~(I2C_SDA_PIN + I2C_SCL_PIN);      // set pins to GPIO
    P3DIR |= I2C_SDA_PIN + I2C_SCL_PIN;         // set output direction
    P3OUT |= I2C_SDA_PIN;                       // set SDA HIGH
    for (i = 0 ; i < 9 ; i++){
        P3OUT |= I2C_SCL_PIN;                   // toggle SCL 9 times
        // TOD: delay
        P3OUT &= ~I2C_SCL_PIN;
        // TOD: delay
    }
    P3SEL |= I2C_SDA_PIN + I2C_SCL_PIN;         // set pins back to I2C
}

/*inline void i2c_handle_rx_interrupt(){
    // Master & Slave Mode: when NACK is detected
    if (UCB0STAT & UCNACKIFG){            // send STOP if slave sends NACK
        if (UCB0CTL0 & UCMST){
            UCB0CTL1 |= UCTXSTP;
        }
    UCB0STAT &= ~UCNACKIFG;
    //  i2c_disableRXInterrupt();
    //  TA0CCTL0 |= CCIE;
    //  __bic_SR_register_on_exit(LPM3_bits);
    }

    // Slave mode: when start condition is detected
    if (UCB0STAT & UCSTTIFG){
        UCB0STAT &= ~UCSTTIFG;                    // Clear start condition int flag
        // initialize something
        slaveData = defaultSlaveData;
        slaveIndex = defaultSlaveIndex;
    }
}

#pragma vector = USCIAB0TX_VECTOR
__interrupt void USCIAB0TX_ISR(void)
{
    // uart interrupt
    if ((IFG2 & UCA0TXIFG) && (IE2 & UCA0TXIE)){
        IE2 &= ~UCA0TXIE;
    }

    // i2c RX interrupt
    if (IFG2 & UCB0RXIFG){
        // Master
        if (UCB0CTL0 & UCMST) {
            //easier when we count down and compare for the one last byte to initiate STOP condition
            masterRxIndex--;
            if (masterRxIndex){
                *masterRxData = UCB0RXBUF;
                masterRxData++;
                if (masterRxIndex == 1)
                    UCB0CTL1 |= UCTXSTP;

            } else {
                *masterRxData = UCB0RXBUF;

                // when all bytes we want are received
                // TOD: make separate process for the below
    //          i2c_disableRXInterrupt();
    //          TA0CCTL0 |= CCIE;
                // Back to Low Power Mode
                i2cRxDone = 1;
                __bic_SR_register_on_exit(LPM3_bits);
            }
        } else {
            // Slave
            if(slaveIndex-1){
                // check the first data is matched to beacon packet address and store the address value for later use
                if (slaveIndex == defaultSlaveIndex)
                {
                    beaconPacketAddress = UCB0RXBUF;
                    if(i2c_checkInternalAddress(beaconPacketAddress))
                    {
                        // store the address value separated from the buffer
                        //beaconPacketAddress = *slaveRXData;
                        // change the data pointer position
                        // TOD: check whether below is correct
                        slaveData = slaveData + beaconPacketAddress*8;
                        slaveIndex = 8;
                    } else {
                        // set to default address
                        beaconPacketAddress = 0x0;
                        // NOT the internal address we want
                        // release the bus (send NACK)
                        // TOD:how do we know when NACK is out??
                        UCB0CTL1 |= UCTXNACK;
                        // TOD: initialize variables
                    }
                } else {
                    *slaveData = UCB0RXBUF;
                    slaveData++;
                    slaveIndex --;
                }
            } else {
                // when we receive all data
                *slaveData = UCB0RXBUF;
                i2cRxDone = 1;
                __bic_SR_register_on_exit(LPM3_bits);
            }
        }
    }

    // i2c TX interrupt
    if (IFG2 & UCB0TXIFG){
        // Master
        if (UCB0CTL0 & UCMST){
            //easier when we count down and compare for the one last byte to initiate STOP condition
            if (masterTxIndex){
                UCB0TXBUF=*masterTxData;
                masterTxData++;
                // When we send only one byte, do this after sending one byte
                masterTxIndex--;
            } else {
                // When we send only one byte, do this after sending one byte
                UCB0CTL1 |= UCTXSTP;                // Generate I2C stop condition right after sending last data
                IFG2 &= ~UCB0TXIFG;

                // send last byte
                // UCB0TXBUF=*masterTxData;
    //          i2c_disableTXInterrupt();
    //          TA0CCTL0 |= CCIE;
                // Back to Low Power Mode
                i2cTxDone = 1;
                __bic_SR_register_on_exit(LPM3_bits);
            }
        } else {
            // Slave
            if (slaveIndex - 1) {
                // first byte to send?
                if (slaveIndex == defaultSlaveIndex) {
                        slaveData = slaveData + beaconPacketAddress*8;
                        slaveIndex = 8;
                        UCB0TXBUF = *slaveData;
                        slaveData++;
                        slaveIndex --;
                } else {
                    UCB0TXBUF = *slaveData;
                    slaveData++;
                    slaveIndex --;
                }
            } else {
                //UCB0CTL1 |=UCTXNACK;
                // when we transmit all the data
                UCB0TXBUF = *slaveData;
                i2cTxDone = 1;
                __bic_SR_register_on_exit(LPM3_bits);
            }
        }
    }
}
*/
