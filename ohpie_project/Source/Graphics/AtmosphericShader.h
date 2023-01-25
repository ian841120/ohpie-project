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
	Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer;

	Microsoft::WRL::ComPtr<ID3D11PixelShader>	pixelShader;
	Microsoft::WRL::ComPtr<ID3D11VertexShader>	vertexShader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>	inputLayout;

	Microsoft::WRL::ComPtr<ID3D11Buffer>		skyCbuffer;


	int indexCount;
private:
	struct Vertex
	{
		DirectX::XMFLOAT3 position;
	};
	struct CbSky
	{
		DirectX::XMFLOAT4X4 world;
		DirectX::XMFLOAT4X4	view_project;
		DirectionLightData directionLightData;	// light position(direction)



	};
private:
	void createDome(const DirectX::XMFLOAT3& startPosition, float radius, int slices, int stacks);
	
};