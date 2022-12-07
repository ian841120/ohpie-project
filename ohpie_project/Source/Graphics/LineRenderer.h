#pragma once
#include <d3d11.h>
#include <directxmath.h>
#include <wrl.h>
#include <vector>
class LineRenderer
{
public:
	LineRenderer(ID3D11Device* device,UINT vertexCount);
	~LineRenderer() {};
public:
	void AddVertex(const DirectX::XMFLOAT3& position, const DirectX::XMFLOAT4& color);
	void Render(ID3D11DeviceContext* device_context, const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection);
private:

	struct Vertex
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT4 color;
		
	};
	struct Cbuffer
	{
		DirectX::XMFLOAT4X4 world_view_projection;
	};
private:
	Microsoft::WRL::ComPtr<ID3D11VertexShader>	vertex_shader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>	pixel_shader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>	input_layout;

	Microsoft::WRL::ComPtr<ID3D11Buffer>		vertex_buffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer>		constant_buffer;

	Microsoft::WRL::ComPtr<ID3D11BlendState>		blend_state;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState>	depth_stencil_state;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState>	rasterizer_state;

	std::vector<Vertex>	vertices;
	UINT MaxVertexCount = 0;
};