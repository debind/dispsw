#include <stdio.h>
#include <string.h>

#include <dispsw.h>



//******************************************************************************
// main function
//******************************************************************************
int main() 
{
	UINT8 u8MenuStatus = DISPSW_MENU_STABLE;
	UINT8 u8MenuNumber = 0;
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

		if (u8MenuNumber != dispsw_u8GetMenu())
		{
			u8MenuNumber = dispsw_u8GetMenu();
			printf("menu now %d\n",u8MenuNumber);
		}

		if (au8MenuValues[u8MenuNumber] != dispsw_u8GetMenuValue(u8MenuNumber))
		{
			printf("Value%02d  now %d\n",u8MenuNumber, dispsw_u8GetMenuValue(u8MenuNumber));

			//--------------------------------------
			// update out working variables.
			au8MenuValues[u8MenuNumber] = dispsw_u8GetMenuValue(u8MenuNumber);		
			//--------------------------------------
		}

		if ((u8MenuStatus == DISPSW_MENU_STABLE) && (dispsw_u8GetMenuState() == DISPSW_MENU_FLASHING))
		{
	    	u8MenuStatus = dispsw_u8GetMenuState();
			printf(" entered flashing\n");
		}

		if ((u8MenuStatus == DISPSW_MENU_FLASHING) && (dispsw_u8GetMenuState() == DISPSW_MENU_STABLE))
		{
	    	u8MenuStatus = dispsw_u8GetMenuState();
			printf(" released flashing\n");
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
