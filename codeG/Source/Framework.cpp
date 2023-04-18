#include <sstream>
#include "Framework.h"
#include "SceneGame.h"
static SceneGame sceneGame;
// Vertical Sync setting   1=60fps , 0=infinity
static const int syncInterval = 1;
Framework::Framework(HWND hwnd) :hwnd(hwnd),graphics(hwnd),inputClass(hwnd)
{
	sceneGame.Initialize();
}
void Framework::Update(float elapsed_time)
{
	graphics.GetImGuiClass()->NewFrame();
	inputClass.Update();
	sceneGame.Update(elapsed_time);
}
void Framework::Render(float elapsed_time)
{
	sceneGame.Render();
	graphics.GetImGuiClass()->Render();
	//Present back buffer
	graphics.GetSwapChain()->Present(syncInterval, 0);
}
void Framework::CalculateFrameRates()
{
	static int frames = 0;
	static float time_elapsed = 0;
	frames++;
	if ((timer.TotalTime() - time_elapsed) >= 1.0f)
	{
		float fps = static_cast<float>(frames);
		float mspf = 1000.0f / fps;
		std::ostringstream outs;
		outs.precision(6);
		outs << " FPS : " << fps << " / " << "Frame Time : " << mspf << " (ms) ";
		SetWindowTextA(hwnd, outs.str().c_str());
		frames = 0;
		time_elapsed += 1.0f;
	}
}
int Framework::Run()
{
	MSG msg = {};

	while (WM_QUIT != msg.message)
	{
		//Window message
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			timer.Tick();
			CalculateFrameRates();
			float elapsed_time = syncInterval == 0
				? timer.DeltaTime()
				: syncInterval / 60.0f
				;
			Update(elapsed_time);
			Render(elapsed_time);
		}
	}
	return static_cast<int>(msg.wParam);
}
LRESULT CALLBACK Framework::HandleMessage(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (graphics.GetImGuiClass()->HandleMessage(hwnd, msg, wParam, lParam))
		return true;
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
		{
			PostMessage(hwnd, WM_CLOSE, 0, 0);
			break;
		}
	case WM_ENTERSIZEMOVE:
		// WM_EXITSIZEMOVE is sent when the user grabs the resize bars.
		timer.Stop();
		break;
	case WM_EXITSIZEMOVE:
		// WM_EXITSIZEMOVE is sent when the user releases the resize bars.
		// Here we reset everything based on the new window dimensions.
		timer.Start();
		break;
	case WM_MOUSEWHEEL:
		InputClass::Instance().GetMouse().SetWheel(GET_WHEEL_DELTA_WPARAM(wParam));
		break;
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}