#include "ImguiClass.h"
ImGuiClass::ImGuiClass(HWND hwnd, ID3D11Device* device,ID3D11DeviceContext*device_context)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::GetIO().Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\consola.ttf", 18.0f, nullptr, ImGui::GetIO().Fonts->GetGlyphRangesJapanese());
	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX11_Init(device, device_context);
	ImGui::StyleColorsDark();
}
ImGuiClass::~ImGuiClass()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}
void ImGuiClass::Render()
{
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
		
}
void ImGuiClass::NewFrame()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}
LRESULT ImGuiClass::HandleMessage(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return ImGui_ImplWin32_WndProcHandler(hwnd, msg, wParam, lParam);
}
