#pragma once
#include <directxmath.h>
#include "RenderContext.h"
class Light
{
public:
	enum class LIGHTTYPE
	{
		directional,	// directional light
		point,			// point light
	};

public:
	Light(LIGHTTYPE lightType=LIGHTTYPE::directional);
	~Light() {};
public:
	void PushRenderContext(RenderContext& rc)const;

	const DirectX::XMFLOAT3& GetDirection() { return direction; }
	void SetDirection(DirectX::XMFLOAT3 d) { direction = d; }

	const DirectX::XMFLOAT4& GetColor() { return color; }
	void SetColor(DirectX::XMFLOAT4 c) { color = c; }

	void DrawDebugGUI();

private:
	DirectX::XMFLOAT3	direction{ 0.0f,-1.0f,-1.0f };
	DirectX::XMFLOAT4	color{ 1.0f,1.0f,1.0f,1.0f };
	LIGHTTYPE lightType = LIGHTTYPE::directional;
	float range = 20.0f;
};