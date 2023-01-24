#pragma once
#include <d3d11.h>
#include <directxmath.h>
#include <wrl.h>
#include "RenderContext.h"
class AtmosphericShader
{
public:
	AtmosphericShader();
	~AtmosphericShader() {};

	void Render(const RenderContext& rc);
private:
	Microsoft::WRL::ComPtr<ID3D11PixelShader>	pixelShader;
	Microsoft::WRL::ComPtr<ID3D11VertexShader>	vertexShader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>	inputLayout;

	Microsoft::WRL::ComPtr<ID3D11Buffer>		skyCbuffer;
private:
	struct CbSky
	{
		DirectX::XMFLOAT4 viewPosition;
		DirectionLightData	directionLightData;

	};
	
};