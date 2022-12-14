#include "Fog.h"
#include "ImguiClass.h"
void Fog::pushRenderContext(RenderContext& rc)
{
	rc.fogData.color = fogColor;
	rc.fogData.fogStart = fogStart;
	rc.fogData.fogRange = fogRange;
}
void Fog::DrawDebugGui()
{
	ImGui::Begin("Effect");
	if (ImGui::TreeNode("Fog"))
	{
		ImGui::DragFloat("FogStart", &fogStart, 0.1f, 0.0f, FLT_MAX);
		ImGui::DragFloat("FogRange", &fogRange, 0.1f, 0.0f, FLT_MAX);
		ImGui::ColorEdit4("FogColor", &fogColor.x);
		ImGui::TreePop();
	}
	ImGui::End();
}