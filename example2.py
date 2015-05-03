#
# example2.py
# - use the expansion board DISP-SW and the library dispsw from python
# - implement a menu
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


from ctypes import *
from time import sleep

MenuValues0 = 0
MenuValues1 = 0
MenuValues2 = 0
MenuValues3 = 0
MenuValues4 = 0
MenuValues5 = 0
MenuValues6 = 0
MenuValues7 = 0
MenuValues8 = 0
MenuValues9 = 0

mylib = CDLL("/usr/local/lib/libdispsw.so")
mylib.dispsw_Start()

while 1:
	mylib.dispsw_MenuUpdate()

	#--------------------------------------
	# if menu content changed just print out.
	if MenuValues0  != mylib.dispsw_u8GetMenu0Value(): 
		print "Value0  now %d" % mylib.dispsw_u8GetMenu0Value()
	if MenuValues1  != mylib.dispsw_u8GetMenu1Value(): 
		print "Value1  now %d" % mylib.dispsw_u8GetMenu1Value()
	if MenuValues2  != mylib.dispsw_u8GetMenu2Value(): 
		print "Value2  now %d" % mylib.dispsw_u8GetMenu2Value()
	if MenuValues3  != mylib.dispsw_u8GetMenu3Value(): 
		print "Value3  now %d" % mylib.dispsw_u8GetMenu3Value()
	if MenuValues4  != mylib.dispsw_u8GetMenu4Value(): 
		print "Value4  now %d" % mylib.dispsw_u8GetMenu4Value()
	if MenuValues5  != mylib.dispsw_u8GetMenu5Value(): 
		print "Value5  now %d" % mylib.dispsw_u8GetMenu5Value()
	if MenuValues6  != mylib.dispsw_u8GetMenu6Value(): 
		print "Value6  now %d" % mylib.dispsw_u8GetMenu6Value()
	if MenuValues7  != mylib.dispsw_u8GetMenu7Value(): 
		print "Value7  now %d" % mylib.dispsw_u8GetMenu7Value()
	if MenuValues8  != mylib.dispsw_u8GetMenu8Value(): 
		print "Value8  now %d" % mylib.dispsw_u8GetMenu8Value()
	if MenuValues9  != mylib.dispsw_u8GetMenu9Value(): 
		print "Value9  now %d" % mylib.dispsw_u8GetMenu9Value()
	#--------------------------------------

	#--------------------------------------
	# update our working variables.
	MenuValues0 = mylib.dispsw_u8GetMenu0Value()
	MenuValues1 = mylib.dispsw_u8GetMenu1Value()
	MenuValues2 = mylib.dispsw_u8GetMenu2Value()
	MenuValues3 = mylib.dispsw_u8GetMenu3Value()
	MenuValues4 = mylib.dispsw_u8GetMenu4Value()
	MenuValues5 = mylib.dispsw_u8GetMenu5Value()
	MenuValues6 = mylib.dispsw_u8GetMenu6Value()
	MenuValues7 = mylib.dispsw_u8GetMenu7Value()
	MenuValues8 = mylib.dispsw_u8GetMenu8Value()
	MenuValues9 = mylib.dispsw_u8GetMenu9Value()
	#--------------------------------------
	
	sleep(0.03)

mylib.dispsw_Stop()
