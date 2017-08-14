#! /usr/bin/env python
# -*- coding: UTF-8 -*-

import serial
from struct import unpack
import numpy as np

stream = serial.Serial( '/dev/tty.usbmodem411', 115200 )  # open first serial port
print stream.portstr       # check which port was really used
line = stream.readline()   # read a '\n' terminated line
print line

LOW = 0
HIGH = 1

threshold = 256/2

if ( line.find( "Girino ready" ) != -1 ):

	print "Girino tells me that it is ready"

	stream.write('d')

	for i in range(6):
		print stream.readline()

	stream.write('p128')
	print stream.readline()

	stream.write('t50')
	print stream.readline()

	stream.write('d')

	for i in range(6):
		print stream.readline()

	eventData = []

	for i in range(5):
		stream.write('s')
		eventString = stream.read(1280)
		eventData.append(np.array(unpack( '1280B', eventString )))
		print eventData
		stream.write('S')
		stream.flushInput()

	np.savetxt( "data.txt", eventData[4] )

	i = 0
	status = LOW
	delta = []

	for datum in eventData[4]:
		if datum > threshold and status == LOW:
			status = HIGH
			pre = i
		if datum < threshold and status == HIGH:
			status = LOW
			delta.append(i - pre)
		i += 1

	np.savetxt( "deltas.txt", delta )
			
		
stream.flushOutput()
stream.flushInput()
stream.close()             # close port
