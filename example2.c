#include <stdio.h>
#include <string.h>

#include <dispsw.h>



//******************************************************************************
// main function
//******************************************************************************
int main() 
{
	UINT8 u8Flash=0;
	UINT8 u8MenuNumber;
	UINT8 u8Encoder;
	UINT8 au8MenuValues[100]= {0};

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
		for (u8MenuNumber = 0; u8MenuNumber < 100; u8MenuNumber++)
		{
			if (au8MenuValues[u8MenuNumber]  != dispsw_u8GetMenuValue(u8MenuNumber)) printf("Value%02d  now %d\n",u8MenuNumber, dispsw_u8GetMenuValue(u8MenuNumber));

			//--------------------------------------
			// update out working variables.
			au8MenuValues[u8MenuNumber] = dispsw_u8GetMenuValue(u8MenuNumber);		
			//--------------------------------------
		}

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
