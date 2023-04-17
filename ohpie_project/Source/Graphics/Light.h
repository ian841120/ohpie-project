#pragma once
#include <directxmath.h>
#include "RenderContext.h"

static constexpr char* lightTypeName[] =
{
	"Directional",
	"Point",
	"Spot"
};

class Light
{
public:
	enum class LIGHTTYPE
	{
		directional,	// directional light
		point,			// point light
		spot,
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
	const bool GetRemove() { return remove; }

	void SetRange(float r) { range = r; }

	float GetInnerCone() { return innerCone; }
	void SetInnerCone(float innerCone) { this->innerCone = innerCone; }
	float GetOuterCone() { return outerCone; }
	void SetOuterCone(float outerCone) { this->outerCone = outerCone; }

	
	//Draw ImGUI
	void DrawDebugGUI();
	void DrawDebugPrimitive();
private:
	// Light Position
	DirectX::XMFLOAT3	position{ 0.0f,0.0f,0.0f };
	// Light Direction 
	DirectX::XMFLOAT3	direction{ 0.0f,-1.0f,0.0f };
	// Light Color
	DirectX::XMFLOAT4	color{ 1.0f,1.0f,1.0f,1.0f };
	LIGHTTYPE lightType = LIGHTTYPE::directional;
	// Point Light Range
	float range = 20.0f;
	bool remove = false;
	float innerCone = 0.99f;
	float outerCone = 0.9f;

	
};