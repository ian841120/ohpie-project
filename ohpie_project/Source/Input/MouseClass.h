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
	void Update();
	float GetPreCursorPositionX()const { return cursorPosition[0].x; }
	float GetPreCursorPositionY()const { return cursorPosition[0].y; }
	float GetCursorPositionX() const { return cursorPosition[1].x; }
	float GetCursorPositionY() const { return cursorPosition[1].y; }
	int GetButtonDown(MOUSEKEY key) { return down[static_cast<int>(key)]; }
	int GetButtonUp(MOUSEKEY key) { return release[static_cast<int>(key)]; }
	int GetButtonState(MOUSEKEY key) { return state[static_cast<int>(key)]; }
	
private:
	HWND	hwnd;
	DirectX::XMFLOAT2 cursorPosition[2] = {};
	int preState[3] = {};
	int state[3] = {};
	int down[3] = {};
	int release[3] = {};

};