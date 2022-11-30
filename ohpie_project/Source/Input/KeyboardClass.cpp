#include "KeyboardClass.h"
#include <windows.h>
void KeyBoardClass::update()
{
	//Update

	if (GetKeyboardState(state))
	{
		for (int i = 0; i < 256; i++)
		{
			//Get the high-order bit of state 
			//If it is 1 the key is down
			//otherwise it is up
			state[i] = state[i] >> 7;
			//	1			*-------*
			//				|		|
			//				|		|
			//	0	  ------*		*-------
			//		 |		|
			//		 |		|
			//	  pre=0	 state=1 ----> down--->1
			// 
			// 	1			*-------*
			//				|		|
			//				|		|
			//	0	  ------*		*-------
			//						|		|
			//						|		|
			//					pre=1	state=0 --->release-->1	

			down[i] = state[i] & ~preState[i];
			release[i] = ~state[i] & preState[i];
			preState[i] = state[i];
			
		}
	}


	

}