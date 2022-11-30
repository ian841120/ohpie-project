#pragma once
#include <windows.h>
#include <d3d11.h>

#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

class ImGuiClass
{
public:
	ImGuiClass(HWND hwnd,ID3D11Device* device, ID3D11DeviceContext* device_context);
	~ImGuiClass();
	void Render();
	void NewFrame();
	LRESULT HandleMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

};