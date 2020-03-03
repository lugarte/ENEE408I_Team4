# This code was modified from http://www.varesano.net/blog/fabio/serial%20rs232%20connections%20python 
# It allows you to enter data to be sent over serial to the arduino.

# If you want to have Serial Monitor open and use this code at the same time, you must run this program before opening serial monitor.

# The port is currently set to Lucie's laptop.
# To change the port for another device, edit port='/dev/cu.usbmodem1411' to another port
# To find what port you need, put into the command line:
# python -m serial.tools.list_ports
# This will display available ports.
# If you are trying to connect to the arduino, the arduino needs to be plugged into your computer for the computer to see it.
# For more help with serial see:
# https://pythonhosted.org/pyserial/shortintro.html#opening-serial-ports

import time
import serial

# configure the serial connections (the parameters differs on the device you are connecting to)
ser = serial.Serial(
		port='/dev/cu.usbmodem1411',
		baudrate=9600,
		parity=serial.PARITY_ODD,
		stopbits=serial.STOPBITS_TWO,
		bytesize=serial.SEVENBITS
		)

ser.isOpen()

print( 'Enter your commands below.\r\nInsert "exit" to leave the application.')

input_read=1
while 1 :
# get keyboard input
	input_read = input(">> ")
	if input_read == 'exit':
		ser.close()
		exit()
	else:
	# send the character to the device
		arr = bytearray(input_read, 'utf-8')
		ser.write(arr)
