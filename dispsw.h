/*
 * dispsw.h:
 * - use the expansion board DISP-SW for the raspberry pi.
 *
 * Copyright (c) 2015 Dennis Binder.
 ***********************************************************************
 * This file is part of libdispsw:
 *
 *    dispsw is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU Lesser General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    dispsw is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU Lesser General Public License for more details.
 *
 *    You should have received a copy of the GNU Lesser General Public License
 *    along with wiringPi.  If not, see <http://www.gnu.org/licenses/>.
 ***********************************************************************
 */


#ifndef _DISPSW_H
#define _DISPSW_H

// ---------------------------------------------------------
// typedefs
typedef unsigned char  UINT8;
typedef char           INT8;
typedef int            INT32;
typedef short          INT16;
typedef unsigned short UINT16;
// ---------------------------------------------------------

// ---------------------------------------------------------
// defines the encoder actions
#define DISPSW_ENCODER_FOREWARD 0x01
#define DISPSW_ENCODER_BACKWARD 0x02
#define dispsw_ENCODER_NOTHING  0
// ---------------------------------------------------------

// ---------------------------------------------------------
// defines the different menu values
#define DISPSW_GET_MENU0_VALUE   u8Menu0Value
#define DISPSW_GET_MENU1_VALUE   u8Menu1Value
#define DISPSW_GET_MENU2_VALUE   u8Menu2Value
#define DISPSW_GET_MENU3_VALUE   u8Menu3Value
#define DISPSW_GET_MENU4_VALUE   u8Menu4Value
#define DISPSW_GET_MENU5_VALUE   u8Menu5Value
#define DISPSW_GET_MENU6_VALUE   u8Menu6Value
#define DISPSW_GET_MENU7_VALUE   u8Menu7Value
#define DISPSW_GET_MENU8_VALUE   u8Menu8Value
#define DISPSW_GET_MENU9_VALUE   u8Menu9Value
#define DISPSW_GET_MENU10_VALUE  u8Menu10Value

// ---------------------------------------------------------
// defines the thread refresh rate [ms]
#define dispsw_THREADCYCLE 2   
// ---------------------------------------------------------

// ---------------------------------------------------------
// defines the flash rate [ms]
#define dispsw_FLASHCYCLE 500   
// ---------------------------------------------------------

// ---------------------------------------------------------
// public functions

// ---------------------------------------------------------
// this functions has to be called first.
// Sets up the gpios to use the board.
// Starts a thread to update the display cyclically.
extern void  dispsw_Start(void);
// ---------------------------------------------------------

// ---------------------------------------------------------
// stops the board and finishes everything.
extern void  dispsw_Stop(void);
// ---------------------------------------------------------

// ---------------------------------------------------------
// Set the numbers onto the display
extern void  dispsw_Set(int Z1, int Z2, int Z3, int Z4);
// ---------------------------------------------------------

// ---------------------------------------------------------
// Set the numbers into flashing mode.
// Each segment can be flashed separately.
extern void  dispsw_Flashing(unsigned char flash1, unsigned char flash2, 
					         unsigned char flash3, unsigned char flash4);
// ---------------------------------------------------------

// ---------------------------------------------------------
// return the state of the encoder.
// return values: DISPSW_ENCODER_FOREWARD = foreward
//                DISPSW_ENCODER_BACKWARD = backward
//                else                    = nothing happened
extern UINT8 dispsw_GetEncoder(void);
// ---------------------------------------------------------

// ---------------------------------------------------------
// return the state of the switch.
// return values: 1 = switch has been pressed
//                0 = nothing happened
extern UINT8 dispsw_GetSwitch(void);
// ---------------------------------------------------------

// ---------------------------------------------------------
// Implements a 10 stage Menu
extern void dispsw_MenuUpdate(void);
// ---------------------------------------------------------


// ---------------------------------------------------------
// these are the different menu values
extern UINT8 dispsw_u8GetMenu0Value(void);
extern UINT8 dispsw_u8GetMenu1Value(void);
extern UINT8 dispsw_u8GetMenu2Value(void);
extern UINT8 dispsw_u8GetMenu3Value(void);
extern UINT8 dispsw_u8GetMenu4Value(void);
extern UINT8 dispsw_u8GetMenu5Value(void);
extern UINT8 dispsw_u8GetMenu6Value(void);
extern UINT8 dispsw_u8GetMenu7Value(void);
extern UINT8 dispsw_u8GetMenu8Value(void);
extern UINT8 dispsw_u8GetMenu9Value(void);
// ---------------------------------------------------------


#endif