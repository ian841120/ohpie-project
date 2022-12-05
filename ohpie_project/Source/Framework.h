#include <wrl.h>
#include <memory>
#include "Graphics/Sprite.h"
#include "Graphics/Graphics.h"
#include "Input\InputClass.h"
#include "CpuTimer.h"
#include "CameraController.h"
class Framework
{
public:
	Framework(HWND);
	~Framework() {};
private:
	void Update(float elapsed_time);
	void Render(float elapsed_time);
	void CalculateFrameRates();
public:
	int Run();
	LRESULT CALLBACK HandleMessage(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
private:
	Graphics			graphics;
	InputClass			inputClass;
	HWND				hwnd;
	CpuTimer			timer;
	CameraController	cameraController;
};
