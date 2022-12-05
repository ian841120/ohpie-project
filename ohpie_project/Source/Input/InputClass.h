#pragma once
#include <windows.h>
#include "KeyboardClass.h"
#include "MouseClass.h"
class InputClass
{
public:
	InputClass(HWND hwnd);
	~InputClass() {};
public:
	static InputClass& Instance()	{ return *instance; }
	KeyBoardClass& GetKeyBoard()	{ return keyBoardClass; }
	MouseClass& GetMouse()			{ return mouseClass; }
	void Update();
private:
	static InputClass*	instance;
	KeyBoardClass		keyBoardClass;
	MouseClass			mouseClass;
};