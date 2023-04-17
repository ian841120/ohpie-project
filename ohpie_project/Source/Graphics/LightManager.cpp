#include "LightManager.h"
#include "ImguiClass.h"
LightManager::~LightManager()
{
	Clear();
}
void LightManager::Remove(Light* light)
{
	auto it = std::find(lights.begin(), lights.end(), light);
	if (it != lights.end())
	{
		lights.erase(it);
		delete	light;
	}

}
void LightManager::Register(Light* light)
{
	lights.emplace_back(light);
}
void LightManager::PushRenderContext(RenderContext& rc)
{
	for (Light* light : lights)
	{
		light->PushRenderContext(rc);
	}
}
void LightManager::DrawDebugPrimitive()
{
	for (Light* light : lights)
	{
		light->DrawDebugPrimitive();
	}
}
void LightManager::Clear()
{
	for (Light* light : lights)
	{
		delete light;
	}
	lights.clear();
}
void LightManager::DrawDebugGUI()
{
	ImGui::Begin("Light");
	static const char* current_item = lightTypeName[static_cast<int>(Light::LIGHTTYPE::directional)];
	if (ImGui::BeginCombo("Light Type", current_item))
	{
		for (int i = 0; i < IM_ARRAYSIZE(lightTypeName); i++)
		{
			bool isSelected = (current_item == lightTypeName[i]);
			if (ImGui::Selectable(lightTypeName[i], isSelected))
			{
				current_item = lightTypeName[i];
			}
			if (isSelected)
			{
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}
	
	if (ImGui::Button("Create", { 65, 30 }))
	{
		if (current_item == lightTypeName[static_cast<int>(Light::LIGHTTYPE::directional)])
		{

		}
		if (current_item == lightTypeName[static_cast<int>(Light::LIGHTTYPE::point)])
		{

			Light* light = new Light(Light::LIGHTTYPE::point);
			LightManager::Instance().Register(light);

		}
		if (current_item == lightTypeName[static_cast<int>(Light::LIGHTTYPE::spot)])
		{

			Light* light = new Light(Light::LIGHTTYPE::spot);


			LightManager::Instance().Register(light);

		}

	}
	int nodeID = 0;
	for (Light* light : lights)
	{
		ImGui::PushID(nodeID++);
		light->DrawDebugGUI();
		if (light->GetRemove())
		{
			Remove(light);
		}
		ImGui::PopID();
	}
	ImGui::End();

}