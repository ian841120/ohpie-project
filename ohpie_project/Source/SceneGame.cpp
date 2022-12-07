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
		if (ImGui::TreeNode("Cylinder"))
		{
			ImGui::SliderFloat3("Position", &cylinder.position.x, -30.0f, 30.0f);
			ImGui::ColorEdit4("Color", &cylinder.color.x);
			ImGui::SliderFloat("Radius", &cylinder.radius, 1, 30);
			ImGui::SliderFloat("Height", &cylinder.height, 1, 30);
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
	//Render
	Camera& camera = Camera::Instance();

	graphics.GetGeometricPrimitive()->DrawPrimitiveCuboid(cuboid.position, cuboid.length, cuboid.height, cuboid.width, cuboid.color);
	graphics.GetGeometricPrimitive()->DrawPrimitiveCylinder(cylinder.position, cylinder.radius, cylinder.height, cylinder.color);
	graphics.GetGeometricPrimitive()->Render(dc, camera.GetView(), camera.GetProjection(), light.direction);


	DrawGrid();
	graphics.GetLineRenderer()->Render(dc, camera.GetView(), camera.GetProjection());
	

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