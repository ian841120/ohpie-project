#include "InputClass.h"
InputClass* InputClass::instance = nullptr;
InputClass::InputClass(HWND hwnd):mouseClass(hwnd)
{
	instance = this;
	
}
void InputClass::update()
{
	keyBoardClass.update();
	mouseClass.update();
}