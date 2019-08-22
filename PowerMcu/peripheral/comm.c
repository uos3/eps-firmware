/*
 * comm.c
 *
 *  Created on: 9 Oct 2015
 *      Author: Tom
 *			Altered by: Tom Piga
 */

#include <hal/stopwatch.h>
#include <peripheral/errormon.h>
#include "hal/uart.h"
#include "hal/i2c.h"
#include <stdint.h>

#include "register.h"
#include "util/crc.h"

static void process_packet(uint8_t *message);
static void respond(uint8_t regId, uint8_t count);
static void clear_buffer();

static uint8_t responseBuffer[128] = {0};
static uint8_t msgBufferLength = 0;

void comm_init()
{
	uart_init();

	// Initialise I2C communication
	i2c_masterInit(I2C_CLOCKSOURCE_SMCLK, 80, I2C_TRANSMIT_MODE);
//      UCB0I2CIE = UCNACKIE;
	i2c_enableRXInterrupt();
	i2c_enableTXInterrupt();
}

void comm_process()
{
    static uint8_t msgBuffer[5] = {0};

	uint8_t len;
	while ((len = uart_rx(msgBuffer, msgBufferLength, 5 - msgBufferLength)) != 0)
	{
	    stopwatch_start(TICKS_PER_SECOND / 100, &clear_buffer);
		msgBufferLength += len;
		if (msgBufferLength == 5)
		{
			stopwatch_disable();
			process_packet(msgBuffer);
			msgBufferLength = 0;
		}
	}
}

static void clear_buffer()
{
	if (msgBufferLength != 0)
	{
		msgBufferLength = 0;
		errormon_increment(PacketTimeout);
	}
}

typedef struct
{
	uint8_t write;
	uint8_t regId;
	uint16_t value;
	uint16_t crc;
} PACKET;

static PACKET packet_parse(uint8_t *msg)
{
	return (PACKET){
		.write = msg[0] >> 7,//& 0x1,
		.regId = (msg[0] & 0x7F), //>>1),
		.value = ((uint16_t)msg[2] << 8) | msg[1],
		.crc = ((uint16_t)msg[4] << 8) | msg[3]
	};
}

static void process_packet(uint8_t *msg)
{
	const PACKET pkt = packet_parse(msg);
	uint16_t calculatedCrc = crc_generate(msg, 0, 3);

	if (pkt.crc != calculatedCrc)
	{
		errormon_increment(CrcInvalid);
		return;
	}

	uint8_t responseRegCount;
	if (pkt.write)
	{
		register_set(pkt.regId, pkt.value);
		responseRegCount = 1;
	}
	else
	{
		responseRegCount = pkt.value;
	}

	respond(pkt.regId, responseRegCount);
}

static void respond(uint8_t regId, uint8_t count)       //THE TOBC is coded to work with only 1 packet at a time
{
	responseBuffer[0] = regId;
	responseBuffer[1] = count;

	uint8_t i;
	for (i = 0; i < count; i++)
	{
		uint16_t reg = register_get(regId + i);
		responseBuffer[2 + i * 2] = reg & 0xFF;
		responseBuffer[3 + i * 2] = (reg >> 8) & 0xFF;
	}

	uint16_t crcOut = crc_generate(responseBuffer, 0, 2 + count * 2);
	responseBuffer[2 + count * 2] = crcOut & 0xFF;
	responseBuffer[3 + count * 2] = (crcOut >> 8) & 0xFF;

	uart_tx(responseBuffer,  4 + count * 2);
}

/* On a UART or an I2C transmission to us, flag the need for processing the
 * incoming packet */
#pragma vector = USCIAB0RX_VECTOR
__interrupt void USCIAB0RX_ISR(void)
{
    // If UART (Port A) interrupt is enabled, and there is a pending interrupt
    if (IFG2 & UCA0RXIFG)  {
        circ_push(&rxBuffer, UCA0RXBUF);
        // Only available in ISR
        core_check_wakeup(UART);
    }
}
    // Master & Slave Mode: when NACK is detected
/*    if (UCB0STAT & UCNACKIFG){            // send STOP if slave sends NACK
        if (UCB0CTL0 & UCMST){
            UCB0CTL1 |= UCTXSTP;
        }
    UCB0STAT &= ~UCNACKIFG;
    */
/*
    // If interrupt was triggered for something else instead, most likely for I2C
    else if (IFG2 & UCB0RXIFG){
        i2c_handle_rx_interrupt();
    }

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
    }*/


#pragma vector = USCIAB0TX_VECTOR
__interrupt void USCIAB0TX_ISR(void)        //Reliant on the assumption that all buffer data will be send before another transmission is required
{
    if (IFG2 & UCA0TXIFG)  {
        if (packetsToSend) {
        UCA0TXBUF = *(txBufferAddress+(6-packetsToSend));
        packetsToSend--;
        } else  {
            IE2 &= ~UCA0TXIE;
        }

    }   else if (IFG2 & UCB0TXIFG)  {
            // Master
            if (UCB0CTL0 & UCMST){
                //easier when we count down and compare for the one last byte to initiate STOP condition
                if (masterTxIndex){
                    UCB0TXBUF=*masterTxData;
                    masterTxData++;
                    masterTxIndex--;
                } else {
                    // When we send only one byte, do this after sending one byte
                    UCB0CTL1 |= UCTXSTP;                // Generate I2C stop condition right after sending last data
                    IE2 &= ~UCB0TXIE;

        //          i2c_disableTXInterrupt();
                  //TA0CCTL0 |= CCIE;
                    // Back to Low Power Mode
                    i2cTxDone = 1;
//                    __bic_SR_register_on_exit(LPM3_bits);
                }
            }
    }   else if (IFG2 & UCB0RXIFG)  {
        // Master
            if (UCB0CTL0 & UCMST) {
                //easier when we count down and compare for the one last byte to initiate STOP condition
                masterRxIndex--;
                if (masterRxIndex){
                    *masterRxData = UCB0RXBUF;
                    masterRxData++; //Accessing the next byte

                } else {
                    *masterRxData = UCB0RXBUF;
                    UCB0CTL1 |= UCTXSTP;

                    // when all bytes we want are received
        //          i2c_disableRXInterrupt();
        //          TA0CCTL0 |= CCIE;
                    // Back to Low Power Mode
                    i2cRxDone = 1;
                    IE2 &= ~UCB0RXIE;

                    //__bic_SR_register_on_exit(LPM3_bits);
                }
            }
    }
}
