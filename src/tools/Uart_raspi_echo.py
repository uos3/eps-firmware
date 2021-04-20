#This code uses UART to loop back information sent to the pi by the MSP430 whilst also printing it
#used to test functionality of Uart driver
import serial
from time import sleep

ser = serial.Serial("dev/ttyS0", 56000) #open port and set baud rate
while True:
  received_data = ser.read() #read serial port
  sleep(0.03)
  print(received_data) #print received data
  command = b'c'
  ser.write(command) #transmit data back 
