#This code uses UART to loop back information sent to the pi by the MSP430 whilst also printing it
#used to test functionality of Uart driver
import serial
from time import sleep

ser = serial.Serial("/dev/ttyUSB0", 56000) #open port and set baud rate
while True:
  last_waiting = 0
  while ser.in_waiting < 3:
    if ser.in_waiting != last_waiting:
      print(ser.in_waiting)
      last_waiting = ser.in_waiting
  received_data = ser.read(3) #read serial port
  sleep(0.03)
  print(received_data) #print received data
  command = b'hello world'
  ser.write(command) #transmit data back 
