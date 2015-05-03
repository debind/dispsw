#
# example1.py
# - use the expansion board DISP-SW and the library dispsw from python
# - print out some numbers and activate different flashing modes.
#
# Copyright (c) 2015 Dennis Binder.
#**********************************************************************
# This file is part of libdispsw:
#
#    dispsw is free software: you can redistribute it and/or modify
#    it under the terms of the GNU Lesser General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    dispsw is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU Lesser General Public License for more details.
#
#    You should have received a copy of the GNU Lesser General Public License
#    along with wiringPi.  If not, see <http://www.gnu.org/licenses/>.
#**********************************************************************


import time
from ctypes import *

Flash = 0
ENCODER_FOREWARD = 1
ENCODER_BACKWARD = 2

mylib = CDLL("/usr/local/lib/libdispsw.so")
mylib.dispsw_Start()

mylib.dispsw_Set(1,2,3,4)

while 1:

	# --------------------------------------
	# Get the state of the encoder and evaluate
	u8Encoder = mylib.dispsw_GetEncoder() 
	if u8Encoder == ENCODER_FOREWARD:
		print "direction was foreward" 
	if u8Encoder == ENCODER_BACKWARD:
		print "direction was backward" 
	# --------------------------------------
	
	#--------------------------------------
	# Get the state of the switch and evaluate
	if mylib.dispsw_GetSwitch():
		print "switch pressed"
		Flash ^= 1;
		# set flashing on for all segments.
		if Flash:
			mylib.dispsw_Flashing(1,1,1,1) 
		# set flashing off
		else:        
			mylib.dispsw_Flashing(0,0,0,0) 
	#--------------------------------------


mylib.dispsw_Stop()
