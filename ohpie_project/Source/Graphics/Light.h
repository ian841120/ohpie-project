#pragma once
#include <directxmath.h>

class Light
{
public:
	enum class LIGHTTYPE
	{
		Directional,
	};

public:
	Light(LIGHTTYPE lightType=LIGHTTYPE::Directional);
	~Light() {};
public:
	const DirectX::XMFLOAT4& GetDirection() { return direction; }
	void SetDirection(DirectX::XMFLOAT4 d) { direction = d; }
	const DirectX::XMFLOAT4& GetColor() { return color; }
	void SetColor(DirectX::XMFLOAT4 c) { color = c; }
	void DrawDebugUI();
private:
	DirectX::XMFLOAT4	direction{ 0.0f,1.0f,1.0f,0.0f };
	DirectX::XMFLOAT4	color{ 1.0f,1.0f,1.0f,1.0f };
	LIGHTTYPE lightType = LIGHTTYPE::Directional;
};