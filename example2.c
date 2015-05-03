#include <stdio.h>
#include <string.h>

#include <dispsw.h>



//******************************************************************************
// main function
//******************************************************************************
int main() 
{
	UINT8 u8Flash=0;
	UINT8 u8Encoder;
	UINT8 u8MenuValues[9]= {0};

	//--------------------------------------
	// - configure the GPIOs to use the board
	// - start display thread
	dispsw_Start();
	//--------------------------------------

	while(1)
	{
		//--------------------------------------
		// process user inputs and update the
		// menu.
		dispsw_MenuUpdate();
		//--------------------------------------

		//--------------------------------------
		// if menu content changed just print out.
		if (u8MenuValues[0]  != DISPSW_GET_MENU0_VALUE) printf("Value0  now %d\n",DISPSW_GET_MENU0_VALUE);
		if (u8MenuValues[1]  != DISPSW_GET_MENU1_VALUE) printf("Value1  now %d\n",DISPSW_GET_MENU1_VALUE);
		if (u8MenuValues[2]  != DISPSW_GET_MENU2_VALUE) printf("Value2  now %d\n",DISPSW_GET_MENU2_VALUE);
		if (u8MenuValues[3]  != DISPSW_GET_MENU3_VALUE) printf("Value3  now %d\n",DISPSW_GET_MENU3_VALUE);
		if (u8MenuValues[4]  != DISPSW_GET_MENU4_VALUE) printf("Value4  now %d\n",DISPSW_GET_MENU4_VALUE);
		if (u8MenuValues[5]  != DISPSW_GET_MENU5_VALUE) printf("Value5  now %d\n",DISPSW_GET_MENU5_VALUE);
		if (u8MenuValues[6]  != DISPSW_GET_MENU6_VALUE) printf("Value6  now %d\n",DISPSW_GET_MENU6_VALUE);
		if (u8MenuValues[7]  != DISPSW_GET_MENU7_VALUE) printf("Value7  now %d\n",DISPSW_GET_MENU7_VALUE);
		if (u8MenuValues[8]  != DISPSW_GET_MENU8_VALUE) printf("Value8  now %d\n",DISPSW_GET_MENU8_VALUE);
		if (u8MenuValues[9]  != DISPSW_GET_MENU9_VALUE) printf("Value9  now %d\n",DISPSW_GET_MENU9_VALUE);
		//--------------------------------------

		//--------------------------------------
		// update out working variables.
		u8MenuValues[0]  = DISPSW_GET_MENU0_VALUE;
		u8MenuValues[1]  = DISPSW_GET_MENU1_VALUE;
		u8MenuValues[2]  = DISPSW_GET_MENU2_VALUE;
		u8MenuValues[3]  = DISPSW_GET_MENU3_VALUE;
		u8MenuValues[4]  = DISPSW_GET_MENU4_VALUE;
		u8MenuValues[5]  = DISPSW_GET_MENU5_VALUE;
		u8MenuValues[6]  = DISPSW_GET_MENU6_VALUE;
		u8MenuValues[7]  = DISPSW_GET_MENU7_VALUE;
		u8MenuValues[8]  = DISPSW_GET_MENU8_VALUE;
		u8MenuValues[9]  = DISPSW_GET_MENU9_VALUE;
		//--------------------------------------

		//--------------------------------------
		// repeat everything after 3ms.
		usleep(3000);
		//--------------------------------------	
	}

	//--------------------------------------
	// stop the display from working. 
	// release CPU power.
	dispsw_Stop();
	//--------------------------------------

	return 0;
}
