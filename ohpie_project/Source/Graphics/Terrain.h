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
	void Update();
	void DebugGUI();
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
	int width = 100, height = 100;

	int octaves = 1;
	float persistence = 1.0f;
	float h = 1.0f;
	float nanika = 0.01f;
};