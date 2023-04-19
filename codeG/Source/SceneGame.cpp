#include "SceneGame.h"
#include "Graphics\Graphics.h"
#include "Input\InputClass.h"

#include <Windows.h>

void SceneGame::Initialize()
{
	player = std::make_unique<Player>();
}
void SceneGame::Update(float elapsed_time)
{

	player->Update(elapsed_time);
	player->DebugGUI();

}
void SceneGame::Render()
{
	Graphics& graphics = Graphics::GetInstance();

	//Get Device context
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();
	ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
	ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();
	
	//Screen clear and set render target view
	FLOAT color[] = { 0.5f, 0.5f, 0.5f, 0.0f };	// RGBA(0.0`1.0)
	dc->ClearRenderTargetView(rtv, color);
	dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	dc->OMSetRenderTargets(1, &rtv, dsv);
	RenderContext rc;
	rc.deviceContext = dc;
	//Render
	player->Render(rc);
}
void SceneGame::Finalize()
{
}
