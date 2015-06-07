/*
 * libdispsw.c:
 * - use the expansion board DISP-SW for the raspberry pi.
 *
 * Copyright (c) 2015 Dennis Binder.
 ***********************************************************************
 * This file is part of libdispsw:
 *
 *    libdispsw is free software: you can redistribute it and/or modify
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

#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

#include "stdlib.h"

#include <wiringPi.h>

#include "dispsw.h"


//******************************************************************************
// DEFINES
//******************************************************************************

// --------------------------------------------------------
// defines the different menus 0-5
enum eMenu
{
	MENU0 = 0,
	MENU1,
	MENU2,
	MENU3,
	MENU4,
	MENU5,
	MENU6,
	MENU7,
	MENU8,
	MENU9 
};
// --------------------------------------------------------


// --------------------------------------------------------
// defines the directions of the encoder and the switch
#define dispsw_ENC_FOREWARD(x)       ((x) & DISPSW_ENCODER_FOREWARD)
#define dispsw_ENC_BACKWARD(x)       ((x) & DISPSW_ENCODER_BACKWARD)
// --------------------------------------------------------

#define MAX_MENU       99
#define MAX_MENU_VALUE 100

// --------------------------------------------------------
// states for the encoder statemchine
enum dispsw_ENCODER_STATE
{
	dispsw_ENC_IDLE  = 0,
	dispsw_ENC_FOREW = 1,
	dispsw_ENC_BACKW = 2,
	dispsw_ENC_EQUAL = 3
};
// --------------------------------------------------------

// --------------------------------------------------------
// macro that tells if we have to switch off the led because
// the falshing mode is on.
#define dispsw_ZIFFER1_FLASH   ((iFlashStatus == 0) && (sThreadDispCmd.aFlash[0] == 1))
#define dispsw_ZIFFER2_FLASH   ((iFlashStatus == 0) && (sThreadDispCmd.aFlash[1] == 1))
#define dispsw_ZIFFER3_FLASH   ((iFlashStatus == 0) && (sThreadDispCmd.aFlash[2] == 1))
#define dispsw_ZIFFER4_FLASH   ((iFlashStatus == 0) && (sThreadDispCmd.aFlash[3] == 1))
// --------------------------------------------------------

// --------------------------------------------------------
// commmands that are executed within the display thread.
enum eDispswCmdtag
{
	IDLE = 0,
	EXIT
}EDISPSWCMD;
// --------------------------------------------------------

// --------------------------------------------------------
// Each information needed within the display thread to display,
// flash, etc the 7-segment elements.
typedef struct sDispswCmdtag
{
	int iDispTrg;
	int iDispCmd;
	int aZiffern[4];
	int aFlash[4];
}SDISPSWCMD;
// --------------------------------------------------------


//******************************************************************************
// static/local functions
//******************************************************************************
static  void  dispsw_configIO          (void);
static  void* dispsw_thread_Display    (void* text);
static  void  dispsw_SetZ0toZ3         (UINT8 u8Z0, UINT8 u8Z1, UINT8 u8Z2, UINT8 u8Z3);
static  void  dispsw_latch             (UINT8 u8Value);
static  void  dispsw_SetZ0toZ3         (UINT8 u8Z0, UINT8 u8Z1, UINT8 u8Z2, UINT8 u8Z3);
static  void  dispsw_SetBCD            (UINT8 u8A, UINT8 u8B, UINT8 u8C, UINT8 u8D);
static  UINT8 dispsw_u8EncProc         (void);

static  void  dispsw_UpdateMenuStable  (UINT8* pu8MenuState, UINT8* pu8Menu, UINT8 u8Encoder);
static  void  dispsw_UpdateMenuFlashing(UINT8* pu8MenuState, UINT8* pu8MenuValue, UINT8 u8Encoder);

//******************************************************************************
// global variables
//******************************************************************************

// --------------------------------------------------------
// used for the display and the encoder / switch
SDISPSWCMD  dispsw_sMainDispCmd = {0};
int*        dispsw_piStatus;
pthread_t   dispsw_threadDisplay;
UINT8       dispsw_u8EncoderState = dispsw_ENC_IDLE;
UINT8       dispsw_u8SwitchD1 = 0;
UINT8       dispsw_u8Encoder  = 0;
INT32       dispsw_i32EncoderValue;
INT32       dispsw_i32EncoderValueOld;
// --------------------------------------------------------

// ---------------------------------------------------------
// these are the different menu values
UINT8 au8MenuValue[100]={0};
// ---------------------------------------------------------


// --------------------------------------------------------
// used for the menu
UINT8 u8Menu        = MENU0;
UINT8 u8MenuState   = DISPSW_MENU_STABLE;
// --------------------------------------------------------

// --------------------------------------------------------
// track the instance 
UINT8 dispsw_u8Instance = 0;
// --------------------------------------------------------


//******************************************************************************
// thread updating the display cyclically
//******************************************************************************
static void* dispsw_thread_Display(void* text)
{
	SDISPSWCMD sThreadDispCmd = {0};
	int iFlashCnt=0;
	int iRunningZiffer;
	int iStatus;
	int iExit;
	int iFlashStatus;
	int iZiffer;
	
	iStatus = 0;
	iExit   = 0;
	iFlashStatus = 0;
	iRunningZiffer = 1;

	while (iExit == 0)
	{
		// --------------------------------------------------------
		// flashing
		iFlashCnt++;
		if (iFlashCnt > dispsw_FLASHCYCLE/dispsw_THREADCYCLE)   iFlashCnt = 0;
		if (iFlashCnt < dispsw_FLASHCYCLE/dispsw_THREADCYCLE/2) iFlashStatus = 1;
		else                                                    iFlashStatus = 0;
		// --------------------------------------------------------

		// --------------------------------------------------------
		// Display Update
		if (dispsw_sMainDispCmd.iDispTrg == 0)
		{
		}
		else if (dispsw_sMainDispCmd.iDispCmd == EXIT)
		{
			iExit = 1;
		}
		else
		{
			// Update whole Display Command
			sThreadDispCmd = dispsw_sMainDispCmd;
			dispsw_sMainDispCmd.iDispTrg = 0;
		}
		// --------------------------------------------------------

		// --------------------------------------------------------
		// Update 7-Segment elements
		switch(iRunningZiffer++)
		{
			case 1:
				iZiffer = sThreadDispCmd.aZiffern[0];
				if (dispsw_ZIFFER1_FLASH) dispsw_SetBCD(1,1,1,1);
				else                      dispsw_SetBCD((iZiffer) & 0x01, (iZiffer>>1) & 0x01, (iZiffer>>2) & 0x01, (iZiffer>>3) & 0x01);				
				dispsw_latch(0);
				dispsw_SetZ0toZ3(1,0,0,0);  
				dispsw_latch(1);
				break;
			case 2:
				iZiffer = sThreadDispCmd.aZiffern[1];
				if (dispsw_ZIFFER2_FLASH) dispsw_SetBCD(1,1,1,1);
				else                      dispsw_SetBCD((iZiffer) & 0x01, (iZiffer>>1) & 0x01, (iZiffer>>2) & 0x01, (iZiffer>>3) & 0x01);				
				dispsw_latch(0);
				dispsw_SetZ0toZ3(0,1,0,0);  
				dispsw_latch(1);
				break;
			case 3:
				iZiffer = sThreadDispCmd.aZiffern[2];
				if (dispsw_ZIFFER3_FLASH) dispsw_SetBCD(1,1,1,1);
				else                      dispsw_SetBCD((iZiffer) & 0x01, (iZiffer>>1) & 0x01, (iZiffer>>2) & 0x01, (iZiffer>>3) & 0x01);				
				dispsw_latch(0);
				dispsw_SetZ0toZ3(0,0,1,0);  
				dispsw_latch(1);
				break;
			case 4:
				iZiffer = sThreadDispCmd.aZiffern[3];
				if (dispsw_ZIFFER4_FLASH) dispsw_SetBCD(1,1,1,1);
				else                      dispsw_SetBCD((iZiffer) & 0x01, (iZiffer>>1) & 0x01, (iZiffer>>2) & 0x01, (iZiffer>>3) & 0x01);				
				dispsw_latch(0);
				dispsw_SetZ0toZ3(0,0,0,1);  
				dispsw_latch(1);
				break;
		}
		if (iRunningZiffer == 5) iRunningZiffer = 1;
		// --------------------------------------------------------

		// --------------------------------------------------------
		// update the encoder
		dispsw_u8Encoder = dispsw_u8EncProc();
		if (dispsw_ENC_FOREWARD(dispsw_u8Encoder)) dispsw_i32EncoderValue++;
		if (dispsw_ENC_BACKWARD(dispsw_u8Encoder)) dispsw_i32EncoderValue--;
		// --------------------------------------------------------

		// --------------------------------------------------------
		// refresh rate
		usleep(dispsw_THREADCYCLE*1000ul);
		// --------------------------------------------------------
	}
	pthread_exit(&iStatus);
}



//******************************************************************************
// Set the display numbers
//******************************************************************************
void dispsw_Set(int Z1, int Z2, int Z3, int Z4)
{
	dispsw_sMainDispCmd.aZiffern[0] = Z1;
	dispsw_sMainDispCmd.aZiffern[1] = Z2;
	dispsw_sMainDispCmd.aZiffern[2] = Z3;
	dispsw_sMainDispCmd.aZiffern[3] = Z4;
	dispsw_sMainDispCmd.iDispCmd = 0;
	dispsw_sMainDispCmd.iDispTrg = 1;
}

//******************************************************************************
// Get the Encoder Direction: 0 = nothing, 1=foreward, 2=backward
//******************************************************************************
UINT8 dispsw_GetEncoder(void)
{
	UINT8 u8RetVal;
	if      (dispsw_i32EncoderValue > dispsw_i32EncoderValueOld) u8RetVal = 1;
	else if (dispsw_i32EncoderValue < dispsw_i32EncoderValueOld) u8RetVal = 2;
	else                                                         u8RetVal = 0;

	dispsw_i32EncoderValueOld = dispsw_i32EncoderValue;
	return u8RetVal;
}

//******************************************************************************
// Return the state of the switch
//******************************************************************************
UINT8 dispsw_GetSwitch(void)
{
	UINT8 u8Switch;
	UINT8 u8RetVal;
	u8Switch = digitalRead(1);

	if ((u8Switch == 1) && (dispsw_u8SwitchD1 == 0)) u8RetVal = 1;
	else                                             u8RetVal = 0;

	dispsw_u8SwitchD1 = u8Switch;
	return u8RetVal;
}

//******************************************************************************
// activate flashing of the display numbers.
// 1 = flashing, 0 = constantly on
//******************************************************************************
void dispsw_Flashing(unsigned char flash1, unsigned char flash2, unsigned char flash3, unsigned char flash4 )
{
	// --------------------------------------------------------
	// write the flashing bits into the display command struct.
	dispsw_sMainDispCmd.aFlash[0] = flash1;
	dispsw_sMainDispCmd.aFlash[1] = flash2;
	dispsw_sMainDispCmd.aFlash[2] = flash3;
	dispsw_sMainDispCmd.aFlash[3] = flash4;
	// --------------------------------------------------------

	// --------------------------------------------------------
	// Trigger the display thread so that it updates the contents
	dispsw_sMainDispCmd.iDispCmd  = 0; // 0 = no command, 1 = exit 
	dispsw_sMainDispCmd.iDispTrg  = 1;
	// --------------------------------------------------------
}

//******************************************************************************
// start the display
//******************************************************************************
void dispsw_Start(void)
{
	if (wiringPiSetup() == -1) return;

	if (dispsw_u8Instance == 1) return;

	dispsw_u8Instance++;

	// --------------------------------------------------------
	// configure the gpios for use
	dispsw_configIO();
	// --------------------------------------------------------

	// --------------------------------------------------------
	// update the display
	dispsw_sMainDispCmd.iDispTrg    = 1;
	// --------------------------------------------------------

	// --------------------------------------------------------
	// run the thread which updates the display cyclically.
	if (pthread_create(&dispsw_threadDisplay, NULL, dispsw_thread_Display, NULL))
	{
		fprintf(stderr, "pthread: pthread_create() failed.\n");
	}
	// --------------------------------------------------------

	dispsw_i32EncoderValueOld = dispsw_i32EncoderValue;
}


//******************************************************************************
// stop the display
//******************************************************************************
void dispsw_Stop(void)
{
	// --------------------------------------------------------
	// set the display off
	dispsw_Set(15,15,15,15);
	usleep(10000);
	// --------------------------------------------------------

	// --------------------------------------------------------
	// try to finish the thread
	dispsw_sMainDispCmd.iDispCmd = EXIT;
	dispsw_sMainDispCmd.iDispTrg = 1;
	// --------------------------------------------------------

	// --------------------------------------------------------
	// wait for the thread to finish
	if (pthread_join(dispsw_threadDisplay, (void*)&dispsw_piStatus))
	{
		fprintf(stderr, "pthread: pthread_join() failed.\n");
	}
	// --------------------------------------------------------
}


//******************************************************************************
// configure the gpios for the DISP-SW board.
//******************************************************************************
static void dispsw_configIO(void)
{
  pinMode(8,  OUTPUT);
  pinMode(9,  OUTPUT);
  pinMode(7,  OUTPUT);
  pinMode(0,  OUTPUT);
  pinMode(2,  OUTPUT);
  pinMode(3,  OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(14, OUTPUT);
  pinMode(15, INPUT);
  pinMode(16, INPUT);
  pinMode(1 , INPUT);
}


//******************************************************************************
// activate the current number 
//******************************************************************************
static void dispsw_latch(UINT8 u8Value)
{
  digitalWrite(7, u8Value);
}

//******************************************************************************
// set the 7-segment elements on or off.
//******************************************************************************
static void dispsw_SetZ0toZ3(UINT8 u8Z0, UINT8 u8Z1, UINT8 u8Z2, UINT8 u8Z3)
{
	digitalWrite(14, u8Z3);
	digitalWrite(13, u8Z0);
	digitalWrite(12, u8Z1);
	digitalWrite(3 , u8Z2);
}

//******************************************************************************
// set the BCD-number pin by pin
//******************************************************************************
static void dispsw_SetBCD(UINT8 u8A, UINT8 u8B, UINT8 u8C, UINT8 u8D)
{
	digitalWrite(8, u8A);
	digitalWrite(2, u8B);
	digitalWrite(0, u8C);
	digitalWrite(9, u8D);
}


//******************************************************************************
// Update the encoder statemachine
//******************************************************************************
static UINT8 dispsw_u8EncProc(void)
{
	UINT8 u8Action=dispsw_ENC_IDLE;
	UINT8 u8Encoder=0;
	u8Encoder  = digitalRead(15)<<1;
	u8Encoder |= digitalRead(16);

	switch (dispsw_u8EncoderState)
	{
		case dispsw_ENC_IDLE:
			if      (u8Encoder == 2) dispsw_u8EncoderState = dispsw_ENC_FOREW;
			else if (u8Encoder == 1) dispsw_u8EncoderState = dispsw_ENC_BACKW;			   
			break;
		case dispsw_ENC_FOREW:
			if      (u8Encoder == 3) 
			{
				dispsw_u8EncoderState = dispsw_ENC_EQUAL;
				u8Action = dispsw_ENC_FOREW;
			}
			else if (u8Encoder == 0) dispsw_u8EncoderState = dispsw_ENC_IDLE;			   
			break;
		case dispsw_ENC_BACKW:
			if      (u8Encoder == 3)
			{
				dispsw_u8EncoderState = dispsw_ENC_EQUAL;
				u8Action = dispsw_ENC_BACKW;
			}
			else if (u8Encoder == 0) dispsw_u8EncoderState = dispsw_ENC_IDLE;			   
			break;
		case dispsw_ENC_EQUAL:
			if      (u8Encoder == 0) dispsw_u8EncoderState = dispsw_ENC_IDLE;
			break;
	}

	return u8Action ;
}


//******************************************************************************
// Update the encoder statemachine
//******************************************************************************
void dispsw_MenuUpdate(void)
{
	UINT8 u8Encoder;

	//-----------------------------------
	// get new encoder values
	u8Encoder = dispsw_GetEncoder();
	//-----------------------------------

	if (u8MenuState == DISPSW_MENU_STABLE) dispsw_UpdateMenuStable  (&u8MenuState, &u8Menu, u8Encoder);
	else                                  dispsw_UpdateMenuFlashing(&u8MenuState, &au8MenuValue[u8Menu], u8Encoder);
	dispsw_Set(u8Menu/10,u8Menu%10,(au8MenuValue[u8Menu]%100)/10,au8MenuValue[u8Menu]%10);

}

//******************************************************************************
// Updates the menu when in STABLE-MODE
//******************************************************************************
static void dispsw_UpdateMenuStable(UINT8* pu8MenuState, UINT8* pu8Menu, UINT8 u8Encoder)
{
	if      (dispsw_GetSwitch())
	{
		u8MenuState = DISPSW_MENU_FLASHING;
		dispsw_Flashing(0,0,1,1);
	}
	else if (u8Encoder == DISPSW_ENCODER_FOREWARD) *pu8Menu = *pu8Menu + 1;
	else if (u8Encoder == DISPSW_ENCODER_BACKWARD) *pu8Menu = *pu8Menu - 1;

	if      (*pu8Menu > MAX_MENU_VALUE) *pu8Menu = 0;
	else if (*pu8Menu > MAX_MENU      )  *pu8Menu = MAX_MENU;
}

//******************************************************************************
// Updates the menu when in FLASHING-MODE
//******************************************************************************
static void dispsw_UpdateMenuFlashing(UINT8* pu8MenuState, UINT8* pu8MenuValue, UINT8 u8Encoder)
{
	if      (dispsw_GetSwitch())     
	{
		*pu8MenuState = DISPSW_MENU_STABLE;	
		dispsw_Flashing(0,0,0,0);
	}
	else if (u8Encoder == DISPSW_ENCODER_FOREWARD)
	{
		(*pu8MenuValue)++;
		if (*pu8MenuValue > MAX_MENU_VALUE) *pu8MenuValue = 0;
	}
	else if (u8Encoder == DISPSW_ENCODER_BACKWARD)
	{
		(*pu8MenuValue)--;
		if (*pu8MenuValue > MAX_MENU_VALUE) *pu8MenuValue = MAX_MENU_VALUE;
	} 
}


//******************************************************************************
// return the menu values
//******************************************************************************
UINT8 dispsw_u8GetMenuValue(UINT8 u8MenuNumber) {return au8MenuValue[u8MenuNumber];}


//******************************************************************************
// return the active menu number
//******************************************************************************
UINT8 dispsw_u8GetMenu() {return u8Menu;}

//******************************************************************************
// return the menu state
//******************************************************************************
UINT8 dispsw_u8GetMenuState() {return u8MenuState;}

