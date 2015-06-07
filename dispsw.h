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

// --------------------------------------------------------
// defines the menu state: flashing or stable
enum dispsw_eMenuState
{
	DISPSW_MENU_STABLE = 0,
	DISPSW_MENU_FLASHING
};
// --------------------------------------------------------

// ---------------------------------------------------------
// defines the encoder actions
#define DISPSW_ENCODER_FOREWARD 0x01
#define DISPSW_ENCODER_BACKWARD 0x02
#define dispsw_ENCODER_NOTHING  0
// ---------------------------------------------------------


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
// returns the different menu values
extern UINT8 dispsw_u8GetMenuValue(UINT8 u8MenuNumber);
// ---------------------------------------------------------

// ---------------------------------------------------------
// returns the active menu number
extern UINT8 dispsw_u8GetMenu();
// ---------------------------------------------------------

// ---------------------------------------------------------
// returns the active menu state: MENU_STABLE or MENU_FLASHING
extern UINT8 dispsw_u8GetMenuState();
// ---------------------------------------------------------

#endif