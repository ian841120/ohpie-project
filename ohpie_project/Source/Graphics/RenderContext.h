#pragma once
#include <d3d11.h>
#include <directxmath.h>
struct DirectionLightData
{
	DirectX::XMFLOAT4 direction;
	DirectX::XMFLOAT4 color;
};
struct PointLightData
{
	DirectX::XMFLOAT4	position;
	DirectX::XMFLOAT4	color;
	float				range;
	DirectX::XMFLOAT3	dummy;
};
static constexpr int PointLightMax = 8;

struct RenderContext
{
	
	ID3D11DeviceContext*	deviceContext;

	// Camera
	DirectX::XMFLOAT4		viewPosition;
	DirectX::XMFLOAT4X4		view;
	DirectX::XMFLOAT4X4		projection;
	//Light Data
	DirectionLightData		directionLightData;
	PointLightData			pointLightData[PointLightMax];
	int						pointLightCount = 0;

	
};