#include "Light.h"
#include "ImguiClass.h"
Light::Light(LIGHTTYPE lightType):lightType(lightType)
{
}
void Light::DrawDebugUI()
{
	if (ImGui::TreeNode("Light"))
	{
		ImGui::SliderFloat3("Direction", &direction.x, -30.0f, 30.0f);
		ImGui::ColorEdit4("Direction", &color.x);
		ImGui::TreePop();
	}

}