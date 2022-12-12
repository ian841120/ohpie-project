#include <wrl.h>
#include <d3d11.h>
#include <memory>
#include "ImguiClass.h"
#include "DebugRenderer.h"
#include "GeometricPrimitive.h"
#include "LineRenderer.h"
#include "Skybox.h"
class Graphics
{
public:
	Graphics(HWND);
	~Graphics() {};
public:
	static Graphics&		GetInstance() { return *instance; }
	ID3D11Device*			GetDevice() const { return device.Get(); }
	ID3D11DeviceContext*	GetDeviceContext()const { return device_context.Get(); }
	IDXGISwapChain*			GetSwapChain() const { return swap_chain.Get(); }
	ID3D11RenderTargetView* GetRenderTargetView()const { return render_target_view.Get(); }
	ID3D11DepthStencilView* GetDepthStencilView() const { return depth_stencil_view.Get(); }
	float					GetScreenWidth()const { return screenWidth; }
	float					GetScreenHeight()const { return screenHeight; }

	ImGuiClass*				GetImGuiClass() const { return imguiClass.get(); }
	DebugRenderer*			GetDebugRenderer() const { return debugRenderer.get(); }
	GeometricPrimitive*		GetGeometricPrimitive()const { return geometricPrimitive.get(); }
	LineRenderer*			GetLineRenderer()const { return lineRenderer.get(); }
	Skybox*					GetSkyBox()const { return skyBox.get(); }
private:
	static Graphics*								instance;
	Microsoft::WRL::ComPtr<ID3D11Device>			device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>		device_context;
	Microsoft::WRL::ComPtr<IDXGISwapChain>			swap_chain;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>	render_target_view;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView>	depth_stencil_view;
	Microsoft::WRL::ComPtr<ID3D11Texture2D>			depth_stencil_buffer;

	std::unique_ptr<ImGuiClass>						imguiClass;
	std::unique_ptr<DebugRenderer>					debugRenderer;
	std::unique_ptr<GeometricPrimitive>				geometricPrimitive;
	std::unique_ptr<LineRenderer>					lineRenderer;
	std::unique_ptr<Skybox>							skyBox;
	float screenWidth;
	float screenHeight;
};
