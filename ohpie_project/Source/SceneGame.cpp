#include "SceneGame.h"
#include "Graphics\Graphics.h"
#include "Input\InputClass.h"
#include "Camera.h"
#include "Graphics\LightManager.h"
void SceneGame::Initialize()
{
	// Directional light
	LightManager::Instance().Register(new Light(Light::LIGHTTYPE::directional));
	// Point Light
	Light* light = new Light(Light::LIGHTTYPE::point);
	light->SetPosition(sphere.position);
	light->SetColor(sphere.color);
	LightManager::Instance().Register(light);
}
void SceneGame::Update(float elapsed_time)
{
	//x += 1*elapsed_time;
	angle += 0.01f;


	earth.position = { sphere.position.x + range * cosf(angle), sphere.position.y, sphere.position.z + range * sinf(angle) };
	earth.radius = 5;
	earth.color = white;


	moon.position = { earth.position.x + range * cosf(2*angle)*0.5f, earth.position.y, earth.position.z + range * sinf(2*angle)*0.5f };
	moon.radius = 2;
	moon.color = white;

	ImGui::Begin("ImGUI");
	LightManager::Instance().DrawDebugGUI();
	DrawDebugGUI();
	ImGui::End();

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
	
	RenderContext rc;
	rc.deviceContext = dc;
	rc.viewPosition = { camera.GetEye().x,camera.GetEye().y,camera.GetEye().z,1.0f };
	rc.view = camera.GetView();
	rc.projection = camera.GetProjection();
	LightManager::Instance().PushRenderContext(rc);
	//graphics.GetGeometricPrimitive()->DrawPrimitiveCuboid(cuboid.position, cuboid.length, cuboid.width, cuboid.height, cuboid.angle, cuboid.color);
	graphics.GetSkyBox()->Render(rc);
	//graphics.GetGeometricPrimitive()->DrawPrimitiveCuboid({ 0.0f,-30.0f,0.0f }, 1000, 10, 1000, { 0.0f,0.0f,0.0f }, { 1.0f,1.0f,1.0f,1.0f });


	graphics.GetGeometricPrimitive()->DrawPrimitiveSphere(sphere.position, 10.0f, { 1.0f,0.0f,0.0f,1.0f });
	graphics.GetGeometricPrimitive()->DrawPrimitiveSphere(earth.position, earth.radius, earth.color);
	graphics.GetGeometricPrimitive()->DrawPrimitiveSphere(moon.position, moon.radius, moon.color);
	graphics.GetGeometricPrimitive()->DrawPrimitiveSphere({ sphere.position.x + 4*range * cosf(angle) ,sphere.position.y ,sphere.position.z + 4*range * sinf(angle) }, 2, sphere.color);
	DrawGrid();
	graphics.GetGeometricPrimitive()->Render(rc);
	graphics.GetLineRenderer()->Render(dc, camera.GetView(), camera.GetProjection());
	LightManager::Instance().DrawDebugPrimitive();
	graphics.GetDebugRenderer()->Render(dc, camera.GetView(), camera.GetProjection());
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


	//X Axis 
	for(auto& cuboid:graphics.GetGeometricPrimitive()->GetCuboid())
	{
		graphics.GetLineRenderer()->AddVertex(cuboid.position, { 1,0,0,1 });
		graphics.GetLineRenderer()->AddVertex({ cuboid.position.x + cuboid.length * 2 ,cuboid.position.y ,cuboid.position.z }, { 1,0,0,1 });
		//Y Axis
		graphics.GetLineRenderer()->AddVertex(cuboid.position, { 0,1,0,1 });
		graphics.GetLineRenderer()->AddVertex({ cuboid.position.x,cuboid.position.y + cuboid.height * 2,cuboid.position.z }, { 0,1,0,1 });
		//Z Axis
		graphics.GetLineRenderer()->AddVertex(cuboid.position, { 0,0,1,1 });
		graphics.GetLineRenderer()->AddVertex({ cuboid.position.x,cuboid.position.y,cuboid.position.z + cuboid.width * 2 }, { 0,0,1,1 });

	}

}
void SceneGame::DrawDebugGUI()
{
	//ImGui Update
	{
		if (ImGui::TreeNode("Cuboid"))
		{
			ImGui::SliderFloat3("Position", &cuboid.position.x, -30.0f, 30.0f);
			ImGui::ColorEdit4("Color", &cuboid.color.x);
			ImGui::SliderFloat("Length", &cuboid.length, 1, 30);
			ImGui::SliderFloat("Height", &cuboid.height, 1, 30);
			ImGui::SliderFloat("Width", &cuboid.width, 1, 30);
			ImGui::SliderFloat3("Angle", &cuboid.angle.x, -180.0f, 180.0f);
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Cylinder"))
		{
			ImGui::SliderFloat3("Position", &cylinder.position.x, -30.0f, 30.0f);
			ImGui::ColorEdit4("Color", &cylinder.color.x);
			ImGui::SliderFloat("Radius", &cylinder.radius, 1, 30);
			ImGui::SliderFloat("Height", &cylinder.height, 1, 30);
			ImGui::SliderFloat3("Angle", &cylinder.angle.x, -180.0f, 180.0f);

			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Sphere"))
		{
			ImGui::SliderFloat3("Position", &sphere.position.x, -30.0f, 30.0f);
			ImGui::ColorEdit4("Color", &sphere.color.x);
			ImGui::SliderFloat("Radius", &sphere.radius, 1, 30);
			ImGui::TreePop();
		}
	}

}