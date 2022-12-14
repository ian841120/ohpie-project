#include "LightManager.h"
#include "ImguiClass.h"
LightManager::~LightManager()
{
	Clear();
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

	int nodeID = 0;
	for (Light* light : lights)
	{
		ImGui::PushID(nodeID++);
		light->DrawDebugGUI();
		ImGui::PopID();
	}
	ImGui::End();

}