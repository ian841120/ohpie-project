#pragma once
#include <d3d11.h>
#include <wrl.h>
#include <vector>
#include <directxmath.h>
class GeometricPrimitive
{
public:
	GeometricPrimitive(ID3D11Device* device);
	~GeometricPrimitive() {};
public:
	void Render(ID3D11DeviceContext* device_context, const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& prjection);
	void DrawPrimitiveCuboid(float length, float width, float height);
	void DrawPrimitiveCylinder();
	void DrawPrimitiveSphere();
	void DrawPrimitiveCapsule();
private:
	struct Cbuffer
	{
		DirectX::XMFLOAT4X4	world_view_project;
		DirectX::XMFLOAT4	color;
	};
	struct Vertex
	{
		DirectX::XMFLOAT3	position;
		DirectX::XMFLOAT3	normal;
	};
private:
	Microsoft::WRL::ComPtr<ID3D11Buffer>	constant_buffer;

	Microsoft::WRL::ComPtr<ID3D11Buffer>	cuboid_vertex_buffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer>	cuboid_index_buffer;

	Microsoft::WRL::ComPtr<ID3D11Buffer>	cylinder_vertex_buffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer>	cylinder_index_buffer;

	Microsoft::WRL::ComPtr<ID3D11Buffer>	sphere_vertex_buffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer>	sphere_index_buffer;


	Microsoft::WRL::ComPtr<ID3D11VertexShader>	vertex_shader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>	pixel_shader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>	input_layout;

	Microsoft::WRL::ComPtr<ID3D11BlendState>		blend_state;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState>	depth_stencil_state;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState>	rasterizer_state;

};