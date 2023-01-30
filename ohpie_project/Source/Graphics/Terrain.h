#pragma once
#include <wrl.h>
#include "RenderContext.h"
#include "PerlinNoise.h"
class Terrain
{
public:
	Terrain();
	~Terrain() {};
	void CreateMesh(int width,int height);
	void Render(const RenderContext& rc);
private:
	Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;

	Microsoft::WRL::ComPtr<ID3D11Buffer>	vertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer>	indexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer>	constantBuffer;

private:
	struct Vertex
	{
		DirectX::XMFLOAT3 position;
	};
	struct CBuffer
	{
		DirectX::XMFLOAT4X4 viewProjection;

	};
	UINT indexCount;
	PerlinNoise perlinNoise;
};