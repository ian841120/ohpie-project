#pragma once
#include <d3d11.h>
#include <directxmath.h>
struct FogData
{
	DirectX::XMFLOAT4	color;
	float				fogStart;
	float				fogRange;
	DirectX::XMFLOAT2	padding;
};
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
	DirectX::XMFLOAT3	padding;
};
static constexpr int PointLightMax = 8;
struct MaskData
{
	ID3D11ShaderResourceView* maskTexture;
	float dissolveThreshold;
	float edgeThreshold;
	DirectX::XMFLOAT4 edgeColor;

};
struct SpotLightData
{
	DirectX::XMFLOAT4 position;
	DirectX::XMFLOAT4 direction;
	DirectX::XMFLOAT4 color;
	float	range;
	float	innerCone;
	float	outerCone;
	float	padding;
};
static constexpr int SpotLightMax = 8;
struct SkySimulationData
{

};

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
	SpotLightData			spotLightData[SpotLightMax];
	int						pointLightCount = 0;
	int						spotLightCount = 0;
	//Fog
	FogData					fogData;
	//MaskData
	MaskData				maskData;
	
};