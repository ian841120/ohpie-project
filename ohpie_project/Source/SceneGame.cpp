#include "SceneGame.h"
#include "Graphics\Graphics.h"
#include "Input\InputClass.h"
#include "Camera.h"
#include "Graphics\LightManager.h"

#include <Windows.h>

void SceneGame::Initialize()
{
	LightManager::Instance().Register(new Light(Light::LIGHTTYPE::directional));

	sprite[0] = std::make_unique<Sprite>();
	texture[static_cast<int>(TextureName::cyberpunk)] = std::make_unique<Texture>("./Data/Image/Title.png");
	texture[static_cast<int>(TextureName::nanachi)] = std::make_unique<Texture>("./Data/Image/nanachi.jpg");
	maskTexture = std::make_unique<Texture>("./Data/Image/febucci-dissolve-texture-example.jpg");
	dissolveThreshold = 1.0f;
	edgeThreshold = 0.2f;
	edgeColor = { 1.0f,0.0f,0.0f,1.0f };
	//spheres.emplace_back(sun);
	terrain = std::make_unique<Terrain>();
}
void SceneGame::Update(float elapsed_time)
{
	sprite[0]->Update(100, 100, 500, 500);
	terrain->Update(elapsed_time);

	ImGui::Begin("GeoCreater");
	LightManager::Instance().DrawDebugGUI();
	
	if(ImGui::Combo("test", &var, name))
	{
		//ImGui::EndCombo();
	}
	ImGui::SameLine();
	if (ImGui::Button("press", { 50, 30 }))
	{
		if (var == 0)
		{
			Sphere sphere = { { 1,1,1,1 } ,{ 0,0,0 }, 10,"SPHERE" };
			spheres.emplace_back(sphere);

		}
		if (var == 1)
		{
			Cuboid cuboid = { {1.0f,1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f},5.0f,5.0f,5.0f };
			cuboids.emplace_back(cuboid);

		}
	}
	if (ImGui::Button("Play", { 50, 30 }))
	{
		timer = 0;
		play = true;
	}
	ImGui::SameLine();
	if (ImGui::Button("Stop", { 50, 30 }))
	{
		play = false;
	}
	if (timer > 60)
	{
		timer = 60;
		play = false;
	}
	ImGui::SliderFloat("timer", &timer, 0, 60);
	if (play)
		timer += elapsed_time*20;

	ImGui::InputInt("index", &index);
	for (int i=0;i<spheres.size();i++)
	{
		if (i == index)
		{
			if (play)
				spheres[i].position.x += elapsed_time * 20;
			ImGui::SliderFloat3("Position", &spheres[i].position.x, -30.0f, 30.0f);
			ImGui::ColorEdit4("Color", &spheres[i].color.x);
			ImGui::SliderFloat("Radius", &spheres[i].radius, 1, 30);

		}

	}

	if (ImGui::Button("Clear", { 50, 30 }))
	{
		spheres.clear();
		cuboids.clear();
	}
	// TextureDebugGui
	DrawTextureDebugGUI();

	ImGui::End();
	terrain->DebugGUI();
	fog->DrawDebugGui();
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

	//Render
	Camera& camera = Camera::Instance();
	
	RenderContext rc;
	rc.deviceContext = dc;

	rc.viewPosition = { camera.GetEye().x,camera.GetEye().y,camera.GetEye().z,1.0f };
	rc.view = camera.GetView();
	rc.projection = camera.GetProjection();
	
	LightManager::Instance().PushRenderContext(rc);
	graphics.GetAtmosphericShader()->Render(rc);

	//
	for (const auto& sphere : spheres)
	{
		graphics.GetGeometricPrimitive()->DrawPrimitiveSphere(sphere.position, sphere.radius, sphere.color);
	}
	for (const auto& cuboid : cuboids)
	{
		graphics.GetGeometricPrimitive()->DrawPrimitiveCuboid(cuboid.position, cuboid.length, cuboid.width, cuboid.height, cuboid.angle, cuboid.color);
	}

	graphics.GetGeometricPrimitive()->Render(rc);
	SpriteShader* shader = graphics.GetShader(ShaderId::maskShader);
	sprite[spriteCount]->SetShaderResourceView(texture[textureCount]->GetSRV(), texture[textureCount]->GetWidth(), texture[textureCount]->GetHeight());
	if (DrawTexture)
	{
		shader->Begin(rc);
		rc.maskData.dissolveThreshold = dissolveThreshold;
		rc.maskData.edgeThreshold = edgeThreshold;
		rc.maskData.edgeColor = edgeColor;
		rc.maskData.maskTexture = maskTexture->GetSRV().Get();
		shader->Render(rc, sprite[0].get());
		shader->End(rc);

	}
	SceneGame::DrawGrid();
	graphics.GetLineRenderer()->Render(rc);
	terrain->Render(rc);
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
void SceneGame::DrawTextureDebugGUI()
{
	ImGui::Begin("Texture");
	ImGui::Checkbox("DrawTexture", &DrawTexture);
	
	ImGui::RadioButton(texture[0]->GetTextureName().c_str(), &textureCount, 0);
	ImGui::RadioButton(texture[1]->GetTextureName().c_str(), &textureCount, 1);
	if (ImGui::TreeNode("Shadowmap"))
	{
		ImGui::Text("texture");
		ImGui::Image(texture[textureCount]->GetSRV().Get(), { 256, 256 }, { 0, 0 }, { 1, 1 }, { 1, 1, 1, 1 });

		ImGui::TreePop();
	}

	
	if (ImGui::TreeNode("Mask"))
	{
		if (ImGui::Button("Play", { 50, 30 }))
		{
			dissolveThreshold = 1.0f;
			playDissolve = true;
		}
		ImGui::SameLine();
		if (ImGui::Button("Stop", { 50, 30 }))
		{
			playDissolve = false;
		}
		if (dissolveThreshold<0)
		{
			dissolveThreshold = 0;
			playDissolve = false;
		}
		if (playDissolve)
			dissolveThreshold -= 0.01f;

		ImGui::SliderFloat("Dissolve Threshold", &dissolveThreshold, 0.0f, 1.0f);
		ImGui::SliderFloat("Edge Threshold", &edgeThreshold, 0.0f, 1.0f);
		ImGui::ColorEdit4("Edge Color", &edgeColor.x);
		ImGui::TreePop();
	}

	ImGui::End();

}