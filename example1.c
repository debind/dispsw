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

	//--------------------------------------
	// - configure the GPIOs to use the board
	// - start display thread
	dispsw_Start();
	//--------------------------------------

	//--------------------------------------
	// print out the numbers "1234" onto the 
	// display
	dispsw_Set(1,2,3,4);
	//--------------------------------------

	while(1)
	{
		//--------------------------------------
		// Get the state of the encoder and evaluate
		u8Encoder  = dispsw_GetEncoder();
		if (u8Encoder == DISPSW_ENCODER_FOREWARD)	printf("direction was foreward\n");
		if (u8Encoder == DISPSW_ENCODER_BACKWARD)	printf("direction was backward\n");
		//--------------------------------------

		//--------------------------------------
		// Get the state of the switch and evaluate
		if (dispsw_GetSwitch())  
		{
			printf("switch pressed\n");
			u8Flash ^= 1;
			// set flashing on for all segments.
			if (u8Flash) dispsw_Flashing(1,1,1,1);
			// set flashing off
			else         dispsw_Flashing(0,0,0,0);
		}
		//--------------------------------------

		//--------------------------------------
		// repeat everything after 30ms.
		usleep(30000);
		//--------------------------------------	
	}

	//--------------------------------------
	// stop the display from working. 
	// release CPU power.
	dispsw_Stop();
	//--------------------------------------

	return 0;
}
