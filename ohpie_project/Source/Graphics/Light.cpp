#include "Light.h"
#include "ImguiClass.h"
#include "Graphics.h"
Light::Light(LIGHTTYPE lightType):lightType(lightType)
{
}
void Light::DrawDebugGUI()
{
	static constexpr char* lightTypeName[] =
	{
		"Directional",
		"Point",
	};

	
	if (ImGui::TreeNode(lightTypeName[static_cast<int>(lightType)]))
	{
		switch (lightType)
		{
		case LIGHTTYPE::directional:
			
			//direction.y += 5.0f;
			ImGui::SliderFloat3("Direction", &direction.x, -5000.0f, 5000.0f);
			ImGui::ColorEdit4("Color", &color.x);
			break;
		case LIGHTTYPE::point:
			ImGui::SliderFloat3("Position", &position.x, -30.0f, 30.0f);
			ImGui::ColorEdit4("Color", &color.x);
			ImGui::DragFloat("Range", &range, 0.1f, 0, FLT_MAX);
			break;
		}
		ImGui::TreePop();
	}

}
void Light::DrawDebugPrimitive()
{
	DebugRenderer* debugRenderer = Graphics::GetInstance().GetDebugRenderer();
	switch (lightType)
	{
	case LIGHTTYPE::directional:
		break;
	case LIGHTTYPE::point:
		debugRenderer->DrawSphere(position, range, color);
		break;
	}
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
		rc.pointLightData[rc.pointLightCount].position.x = position.x;
		rc.pointLightData[rc.pointLightCount].position.y = position.y;
		rc.pointLightData[rc.pointLightCount].position.z = position.z;
		rc.pointLightData[rc.pointLightCount].position.w = 0.0f;
		rc.pointLightData[rc.pointLightCount].range = range;
		rc.pointLightData[rc.pointLightCount].color = color;
		rc.pointLightCount++;
		break;
	}
}