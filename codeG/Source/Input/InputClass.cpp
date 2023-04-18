#include "InputClass.h"
InputClass* InputClass::instance = nullptr;
InputClass::InputClass(HWND hwnd):mouseClass(hwnd)
{
	instance = this;
	
}
void InputClass::Update()
{
	keyBoardClass.Update();
	mouseClass.Update();
}