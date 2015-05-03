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
		if (u8MenuValues[0]  != dispsw_u8GetMenu0Value()) printf("Value0  now %d\n",dispsw_u8GetMenu0Value());
		if (u8MenuValues[1]  != dispsw_u8GetMenu1Value()) printf("Value1  now %d\n",dispsw_u8GetMenu1Value());
		if (u8MenuValues[2]  != dispsw_u8GetMenu2Value()) printf("Value2  now %d\n",dispsw_u8GetMenu2Value());
		if (u8MenuValues[3]  != dispsw_u8GetMenu3Value()) printf("Value3  now %d\n",dispsw_u8GetMenu3Value());
		if (u8MenuValues[4]  != dispsw_u8GetMenu4Value()) printf("Value4  now %d\n",dispsw_u8GetMenu4Value());
		if (u8MenuValues[5]  != dispsw_u8GetMenu5Value()) printf("Value5  now %d\n",dispsw_u8GetMenu5Value());
		if (u8MenuValues[6]  != dispsw_u8GetMenu6Value()) printf("Value6  now %d\n",dispsw_u8GetMenu6Value());
		if (u8MenuValues[7]  != dispsw_u8GetMenu7Value()) printf("Value7  now %d\n",dispsw_u8GetMenu7Value());
		if (u8MenuValues[8]  != dispsw_u8GetMenu8Value()) printf("Value8  now %d\n",dispsw_u8GetMenu8Value());
		if (u8MenuValues[9]  != dispsw_u8GetMenu9Value()) printf("Value9  now %d\n",dispsw_u8GetMenu9Value());
		//--------------------------------------

		//--------------------------------------
		// update out working variables.
		u8MenuValues[0]  = dispsw_u8GetMenu0Value();
		u8MenuValues[1]  = dispsw_u8GetMenu1Value();
		u8MenuValues[2]  = dispsw_u8GetMenu2Value();
		u8MenuValues[3]  = dispsw_u8GetMenu3Value();
		u8MenuValues[4]  = dispsw_u8GetMenu4Value();
		u8MenuValues[5]  = dispsw_u8GetMenu5Value();
		u8MenuValues[6]  = dispsw_u8GetMenu6Value();
		u8MenuValues[7]  = dispsw_u8GetMenu7Value();
		u8MenuValues[8]  = dispsw_u8GetMenu8Value();
		u8MenuValues[9]  = dispsw_u8GetMenu9Value();
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
