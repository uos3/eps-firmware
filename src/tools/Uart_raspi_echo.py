#This code uses UART to loop back information sent to the pi by the MSP430 whilst also printing it
#used to test functionality of Uart driver
import serial
from time import sleep

ser = serial.Serial("dev/ttyS0", 56000) #open port and set baud rate
while True:
  received_data = ser.read() #read serial port
  sleep(0.03)
  data_left = ser.inWaiting() #check for remaining bytes
  received_data += ser.read(data_left)
  print(received_data) #print received data 
  ser.write(received_data) #transmit data back 