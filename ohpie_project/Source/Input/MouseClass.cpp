#include "MouseClass.h"
static const int KeyMap[] =
{
	VK_LBUTTON,
	VK_RBUTTON,
	VK_MBUTTON,
};
void MouseClass::Update()
{
	ZeroMemory(state, sizeof(state));
	
	for (int i = 0; i < 3; ++i)
	{
		if (GetAsyncKeyState(KeyMap[i]) & 0x8000)
		{
			state[i] |= (1 << i);
		}
	}
	for (int i = 0; i < 3; i++)
	{
		down[i] = state[i] & ~preState[i];
		release[i] = ~state[i] & preState[i];
		preState[i] = state[i];

	}

	wheel[1] = wheel[0];
	wheel[0] = 0;

	//Get cursor position
	POINT cursor;
	GetCursorPos(&cursor);
	ScreenToClient(hwnd, &cursor);
	cursorPosition[0] = cursorPosition[1];
	cursorPosition[1] = { static_cast<float>(cursor.x), static_cast<float>(cursor.y) };
}