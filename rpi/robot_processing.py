#!/usr/bin/python
import serial
import time

bt = serial.Serial("/dev/rfcomm1", baudrate=9600, timeout=10)

while True:
	print bt.readline()
	#time.sleep(1);
	#bytesToRead = bt.inWaiting()
	#print "bytesToRead="+str(bytesToRead)
	#rx = bt.read(bytesToRead)
	#print rx
