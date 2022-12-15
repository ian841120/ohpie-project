#pragma once
#include <d3d11.h>
#include <wrl.h>
#include <directxmath.h>
#include "RenderContext.h"
class Skybox
{
public:
	Skybox(ID3D11Device* device);
	~Skybox() {};
	void Render(const RenderContext& rc);
private:
	struct Vertex
	{
		DirectX::XMFLOAT3 position;
	};
	struct Cbuffer
	{
		DirectX::XMFLOAT4X4 view_projeciton;
		DirectX::XMFLOAT4X4 world;
	};
private:
	Microsoft::WRL::ComPtr<ID3D11Buffer>	vertex_buffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer>	index_buffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer>	constant_buffer;

	Microsoft::WRL::ComPtr<ID3D11VertexShader>	vertex_shader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>	pixel_shader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>	input_layout;

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> shader_resource_view;

	UINT sphereIndexCount = 0;

};