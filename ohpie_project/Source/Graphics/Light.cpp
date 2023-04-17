#include "Light.h"
#include "ImguiClass.h"
#include "Graphics.h"
Light::Light(LIGHTTYPE lightType):lightType(lightType)
{
}
void Light::DrawDebugGUI()
{

	
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
			if (ImGui::Button("Remove", { 65, 30 }))
			{
				remove = true;
			}
			break;
		case LIGHTTYPE::spot:
			ImGui::SliderFloat3("Position", &position.x, -30.0f, 30.0f);
			ImGui::SliderFloat3("Direction", &direction.x, -30.0f, 30.0f);

			ImGui::ColorEdit4("Color", &color.x);
			ImGui::DragFloat("Range", &range, 0.1f, 0, FLT_MAX);
			ImGui::SliderFloat("innerCone", &innerCone, 0.0f, 1.0f);
			ImGui::SliderFloat("outerCone", &outerCone, 0.0f, 1.0f);
			if (ImGui::Button("Remove", { 65, 30 }))
			{
				remove = true;
			}
			break;
		}
		ImGui::TreePop();
	}

}
void Light::DrawDebugPrimitive()
{
	DebugRenderer* debugRenderer = Graphics::GetInstance().GetDebugRenderer();
	LineRenderer* lineRenderer = Graphics::GetInstance().GetLineRenderer();
	switch (lightType)
	{
	case LIGHTTYPE::directional:
		break;
	case LIGHTTYPE::point:
		debugRenderer->DrawSphere(position, range, color);
		break;
	case LIGHTTYPE::spot:
		DirectX::XMVECTOR Dir = DirectX::XMLoadFloat3(&direction);
		float length;
		DirectX::XMStoreFloat(&length, DirectX::XMVector3Length(Dir));
		if (length < 0.00001f)
			break;
		Dir = DirectX::XMVector3Normalize(Dir);
		DirectX::XMFLOAT3 dir;
		DirectX::XMStoreFloat3(&dir, Dir);
		DirectX::XMVECTOR temp;
		if (fabs(dir.y) == 1.0f)
			temp = DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
		else
			temp = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
		DirectX::XMVECTOR	XAxis = DirectX::XMVector3Cross(temp, Dir);
		DirectX::XMVECTOR	YAxis = DirectX::XMVector3Cross(Dir, XAxis);
		XAxis = DirectX::XMVector3Cross(YAxis, Dir);
		static constexpr int split = 16;
		// Outer
		for (int i = 0; i < split; i++)
		{
			float step = static_cast<float>(i) / static_cast<float>(split);
			float r = -DirectX::XM_PI + DirectX::XM_2PI * step;
			DirectX::XMMATRIX RotationZ = DirectX::XMMatrixRotationAxis(Dir, r);
			DirectX::XMFLOAT3 oldPoint;


			{
				DirectX::XMVECTOR Point = Dir;
				DirectX::XMMATRIX Rotation = DirectX::XMMatrixRotationAxis(XAxis, acosf(outerCone)) * RotationZ;
				Point = DirectX::XMVectorMultiply(Point, DirectX::XMVectorSet(range, range, range, 0));
				Point = DirectX::XMVector3TransformCoord(Point, Rotation);
				Point = DirectX::XMVectorAdd(Point, DirectX::XMLoadFloat3(&position));
				DirectX::XMFLOAT3 pos;
				DirectX::XMStoreFloat3(&pos, Point);
				lineRenderer->AddVertex(position, color);
				lineRenderer->AddVertex(pos, color);
				oldPoint = pos;
			}

			for (int j = 0; j <= split; j++)
			{
				float step = static_cast<float>(j) / static_cast<float>(split);
				float a = outerCone+(1.0f - outerCone) * step;
				DirectX::XMVECTOR Point = Dir;
				DirectX::XMMATRIX Rotation = DirectX::XMMatrixRotationAxis(XAxis, acosf(a)) * RotationZ;
				Point = DirectX::XMVectorMultiply(Point, DirectX::XMVectorSet(range, range, range, 0));
				Point = DirectX::XMVector3TransformCoord(Point, Rotation);
				Point = DirectX::XMVectorAdd(Point, DirectX::XMLoadFloat3(&position));
				DirectX::XMFLOAT3 pos;
				DirectX::XMStoreFloat3(&pos, Point);
				lineRenderer->AddVertex(oldPoint, color);
				lineRenderer->AddVertex(pos, color);
				oldPoint = pos;

			}
		}
		// Inner
		for (int i = 0; i < split; i++)
		{
			float step = static_cast<float>(i) / static_cast<float>(split);
			float r = -DirectX::XM_PI + DirectX::XM_2PI * step;
			DirectX::XMMATRIX RotationZ = DirectX::XMMatrixRotationAxis(Dir, r);
			DirectX::XMFLOAT3 oldPoint;


			{
				DirectX::XMVECTOR Point = Dir;
				DirectX::XMMATRIX Rotation = DirectX::XMMatrixRotationAxis(XAxis, acosf(innerCone)) * RotationZ;
				Point = DirectX::XMVectorMultiply(Point, DirectX::XMVectorSet(range, range, range, 0));
				Point = DirectX::XMVector3TransformCoord(Point, Rotation);
				Point = DirectX::XMVectorAdd(Point, DirectX::XMLoadFloat3(&position));
				DirectX::XMFLOAT3 pos;
				DirectX::XMStoreFloat3(&pos, Point);
				lineRenderer->AddVertex(position, color);
				lineRenderer->AddVertex(pos, color);
				oldPoint = pos;
			}
			for (int j = 0; j <= split; j++)
			{
				float step = static_cast<float>(j) / static_cast<float>(split);
				float a = innerCone + (1.0f - innerCone) * step;
				DirectX::XMVECTOR Point = Dir;
				DirectX::XMMATRIX Rotation = DirectX::XMMatrixRotationAxis(XAxis, acosf(a)) * RotationZ;
				Point = DirectX::XMVectorMultiply(Point, DirectX::XMVectorSet(range, range, range, 0));
				Point = DirectX::XMVector3TransformCoord(Point, Rotation);
				Point = DirectX::XMVectorAdd(Point, DirectX::XMLoadFloat3(&position));
				DirectX::XMFLOAT3 pos;
				DirectX::XMStoreFloat3(&pos, Point);
				lineRenderer->AddVertex(oldPoint, color);
				lineRenderer->AddVertex(pos, color);
				oldPoint = pos;

			}

		}

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
		rc.pointLightData[rc.pointLightCount].position.w = 1.0f;
		rc.pointLightData[rc.pointLightCount].range = range;
		rc.pointLightData[rc.pointLightCount].color = color;
		rc.pointLightCount++;
		break;
	case LIGHTTYPE::spot:
		if (rc.spotLightCount >= SpotLightMax)
			break;
		rc.spotLightData[rc.spotLightCount].position.x = position.x;
		rc.spotLightData[rc.spotLightCount].position.y = position.y;
		rc.spotLightData[rc.spotLightCount].position.z = position.z;
		rc.spotLightData[rc.spotLightCount].position.w = 1.0f;
		rc.spotLightData[rc.spotLightCount].direction.x = direction.x;
		rc.spotLightData[rc.spotLightCount].direction.y = direction.y;
		rc.spotLightData[rc.spotLightCount].direction.z = direction.z;
		rc.spotLightData[rc.spotLightCount].direction.w = 0.0f;
		rc.spotLightData[rc.spotLightCount].color = color;
		rc.spotLightData[rc.spotLightCount].innerCone = innerCone;
		rc.spotLightData[rc.spotLightCount].outerCone = outerCone;
		rc.spotLightData[rc.spotLightCount].range = range;
		rc.spotLightCount++;	
		break;
	}
}