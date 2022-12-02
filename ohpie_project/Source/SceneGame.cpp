#include "SceneGame.h"
#include "Graphics\Graphics.h"
#include "Input\InputClass.h"
#include "Camera.h"

void SceneGame::Initialize()
{
	//sprite[0] = std::make_unique<Sprite>(L"./Data/Image/cyberpunk.jpg");
	//sprite[1] = std::make_unique<Sprite>(L"./Data/Image/player-sprites.png");
	Graphics& graphics = Graphics::GetInstance();
	Camera& camera = Camera::Instance();
	camera.SetLookAt(
		DirectX::XMFLOAT3(0, 75, 75),
		DirectX::XMFLOAT3(0, 0, 0),
		DirectX::XMFLOAT3(0, 1, 0)
	);
	camera.SetPerspectiveFov(
		DirectX::XMConvertToRadians(45),
		1280.0f / 720.0f,
		1.0f,
		10000.0f
	);
	
}
void SceneGame::Update(float elapsed_time)
{
	//KeyBoardClass& keyBoard = InputClass::Instance().GetKeyBoard();
	//x += 1*elapsed_time;
	//ImGui Update
	{
		ImGui::Begin("ImGUI");
		ImGui::SliderFloat("TESTX", &x, 0, 10);
		ImGui::SliderFloat("TESTY", &y, 0, 10);
		ImGui::End();

	}
}
void SceneGame::Render()
{
	Graphics& graphics = Graphics::GetInstance();
	//Get Device context
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();
	ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
	ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();
	//Screen clear and set render target view
	FLOAT color[] = { 0.0f, 0.0f, 0.5f, 1.0f };	// RGBA(0.0�`1.0)
	dc->ClearRenderTargetView(rtv, color);
	dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	dc->OMSetRenderTargets(1, &rtv, dsv);
	Camera& camera = Camera::Instance();

	graphics.GetDebugRenderer()->DrawCylinder({ 0,0,0 }, 10, 10, { 1,1,1,1 });
	graphics.GetDebugRenderer()->Render(dc, camera.GetView(), camera.GetProjection());
	//sprite[0]->SetBlenderState(Sprite::BLENDER_STATE::NONE);
	//sprite[0]->SetSamplerState(Sprite::SAMPLER_STATE::LINEAR_SAMPLER_STATE);
	//sprite[0]->render(dc, 0, 0, 1280, 720);
	//
	//sprite[1]->SetBlenderState(Sprite::BLENDER_STATE::ALPHA);
	//sprite[1]->render(dc, x, y, 200, 200, 1, 1, 1, 1, 0, 0, 0, 140, 240);

}
void SceneGame::Finalize()
{
}