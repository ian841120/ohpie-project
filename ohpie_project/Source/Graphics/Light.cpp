#include "Light.h"
#include "ImguiClass.h"
Light::Light(LIGHTTYPE lightType):lightType(lightType)
{
}
void Light::DrawDebugGUI()
{
	ImGui::Begin("Light");
	if (ImGui::TreeNode("Directtion Light"))
	{
		ImGui::SliderFloat3("Direction", &direction.x, -30.0f, 30.0f);
		ImGui::ColorEdit4("Direction", &color.x);
		ImGui::TreePop();
	}
	ImGui::End();

}
void Light::PushRenderContext(RenderContext& rc)const
{
	switch (lightType)
	{
	case LIGHTTYPE::directional:
		rc.directionLightData.direction.x = direction.x;
		rc.directionLightData.direction.y = direction.y;
		rc.directionLightData.direction.z = direction.z;
		rc.directionLightData.direction.w = 0.0f;
		rc.directionLightData.color = color;
		break;
	case LIGHTTYPE::point:
		if (rc.pointLightCount >= PointLightMax)
			break;
		rc.pointLightData[rc.pointLightCount].direction.x = direction.x;
		rc.pointLightData[rc.pointLightCount].direction.y = direction.y;
		rc.pointLightData[rc.pointLightCount].direction.z = direction.z;
		rc.pointLightData[rc.pointLightCount].direction.w = 0.0f;
		rc.pointLightData[rc.pointLightCount].range = range;
		rc.pointLightData[rc.pointLightCount].color = color;
		rc.pointLightCount++;
		break;
	}
}