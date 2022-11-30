#pragma once
#include <windows.h>
#include <directxmath.h>
class MouseClass
{
public:
	enum class MOUSEKEY:int
	{
		LBUTTON,
		RBUTTON,
		MBUTTON,
	};
public:
	MouseClass(HWND hwnd) :hwnd(hwnd) {};
	~MouseClass() {};
	void update();
	float GetCursorPositionX() { return cursorPosition.x; }
	float GetCursorPositionY() { return cursorPosition.y; }
	int GetButtonDown(MOUSEKEY key) { return down[static_cast<int>(key)]; }
	int GetButtonUp(MOUSEKEY key) { return release[static_cast<int>(key)]; }
	int GetButtonState(MOUSEKEY key) { return state[static_cast<int>(key)]; }
	
private:
	HWND	hwnd;
	DirectX::XMFLOAT2 cursorPosition = {};
	int preState[3] = {};
	int state[3] = {};
	int down[3] = {};
	int release[3] = {};

};