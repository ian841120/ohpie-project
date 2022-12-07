#include "SceneGame.h"
#include "Graphics\Graphics.h"
#include "Input\InputClass.h"
#include "Camera.h"

void SceneGame::Initialize()
{
	//sprite[0] = std::make_unique<Sprite>(L"./Data/Image/cyberpunk.jpg");
	//sprite[1] = std::make_unique<Sprite>(L"./Data/Image/player-sprites.png");
	
}
void SceneGame::Update(float elapsed_time)
{
	//KeyBoardClass& keyBoard = InputClass::Instance().GetKeyBoard();
	//x += 1*elapsed_time;
	angle += 1*elapsed_time;
	//ImGui Update
	{
		ImGui::Begin("ImGUI");
		if (ImGui::TreeNode("Light"))
		{
			ImGui::SliderFloat3("Direction", &light.direction.x, -30.0f, 30.0f);
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Cuboid"))
		{
			ImGui::SliderFloat3("Position", &cuboid.position.x, -30.0f, 30.0f);
			ImGui::ColorEdit4("Color", &cuboid.color.x);
			ImGui::SliderFloat("Length", &cuboid.length, 1, 30);
			ImGui::SliderFloat("Height", &cuboid.height, 1, 30);
			ImGui::SliderFloat("Width", &cuboid.width, 1, 30);
			ImGui::TreePop();
		}
		

		ImGui::End();
		ImGui::Begin("ImGUI2");
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
	FLOAT color[] = { 0.2f, 0.2f, 0.2f, 0.0f };	// RGBA(0.0`1.0)
	dc->ClearRenderTargetView(rtv, color);
	dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	dc->OMSetRenderTargets(1, &rtv, dsv);
	Camera& camera = Camera::Instance();

	graphics.GetGeometricPrimitive()->DrawPrimitiveCuboid(cuboid.position, cuboid.length, cuboid.height, cuboid.width, cuboid.color);
	graphics.GetGeometricPrimitive()->Render(dc, camera.GetView(), camera.GetProjection(), light.direction);


	//graphics.GetDebugRenderer()->DrawCylinder({ -40,0,0 }, r, h, { 1,1,1,1 });
	graphics.GetDebugRenderer()->DrawSphere({ 0,0,0 }, 0.5f, { 1,0,0,1 });
	
	graphics.GetDebugRenderer()->DrawCapsule({ 0,0,0 }, 10, 10, { 1,1,1,1 });
	graphics.GetDebugRenderer()->DrawCapsule({ 10,0,0 }, 10, 10, { 1,1,1,1 });
	graphics.GetDebugRenderer()->DrawCapsule({ 20,0,0 }, 10, 10, { 1,1,1,1 });
	graphics.GetDebugRenderer()->DrawCapsule({ 30,0,0 }, 10, 10, { 0,1,1,1 });
	graphics.GetDebugRenderer()->Render(dc, camera.GetView(), camera.GetProjection());

	DrawGrid();
	graphics.GetLineRenderer()->Render(dc, camera.GetView(), camera.GetProjection());
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
void SceneGame::DrawGrid()
{
	Graphics& graphics = Graphics::GetInstance();
	//X Axis 
	graphics.GetLineRenderer()->AddVertex({ 0,0,0 }, { 1,0,0,1 });
	graphics.GetLineRenderer()->AddVertex({ 75,0,0 }, { 1,0,0,1 });
	//Y Axis
	graphics.GetLineRenderer()->AddVertex({ 0,0,0 }, { 0,1,0,1 });
	graphics.GetLineRenderer()->AddVertex({ 0,75,0 }, { 0,1,0,1 });
	//Z Axis
	graphics.GetLineRenderer()->AddVertex({ 0,0,0 }, { 0,0,1,1 });
	graphics.GetLineRenderer()->AddVertex({ 0,0,75 }, { 0,0,1,1 });

}