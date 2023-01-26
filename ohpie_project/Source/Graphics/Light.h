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
	// Position
	const DirectX::XMFLOAT3& GetPosition() { return position; }
	void SetPosition(DirectX::XMFLOAT3 p) { position = p; }
	// Direction
	const DirectX::XMFLOAT3& GetDirection() { return direction; }
	void SetDirection(DirectX::XMFLOAT3 d) { direction = d; }
	// Color 
	const DirectX::XMFLOAT4& GetColor() { return color; }
	void SetColor(DirectX::XMFLOAT4 c) { color = c; }
	//Draw ImGUI
	void DrawDebugGUI();
	void DrawDebugPrimitive();
private:
	// Light Position
	DirectX::XMFLOAT3	position{ 0.0f,0.0f,0.0f };
	// Light Direction 
	DirectX::XMFLOAT3	direction{ 0.0f,-1.0f,5000.0f };
	// Light Color
	DirectX::XMFLOAT4	color{ 1.0f,1.0f,1.0f,1.0f };
	LIGHTTYPE lightType = LIGHTTYPE::directional;
	// Point Light Range
	float range = 20.0f;
};