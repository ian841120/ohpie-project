#pragma once
#include <d3d11.h>
#include <directxmath.h>
#include <wrl.h>
#include <vector>
class DebugRenderer
{
public:
	DebugRenderer(ID3D11Device* device);
	~DebugRenderer() {};
public:
	//Render
	void Render(ID3D11DeviceContext* device_context, const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4 projection);
	//Draw sphere
	void DrawSphere(const DirectX::XMFLOAT3& center, float radius, const DirectX::XMFLOAT4& color);
	//Draw cylinder
	void DrawCylinder(const DirectX::XMFLOAT3& position, float radius, float height, const DirectX::XMFLOAT4& color);
private:
	void CreateSphereMesh(ID3D11Device* device, float radius, int slices, int stacks);
	void CreateCylinderMesh(ID3D11Device device, float radius1, float radius, float start, float height, int slices, int stacks);
private:
	struct CbMesh
	{
		DirectX::XMFLOAT4X4	view_project;
		DirectX::XMFLOAT4	color;
	};
	struct Sphere
	{
		DirectX::XMFLOAT4	color;
		DirectX::XMFLOAT3	center;
		float				radius;
	};
	struct Cylinder
	{
		DirectX::XMFLOAT4	color;
		DirectX::XMFLOAT3	position;
		float				radius;
		float				height;
	};
	Microsoft::WRL::ComPtr<ID3D11Buffer>	sphereVertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer>	cylinderVertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer>	constantBuffer;

	Microsoft::WRL::ComPtr<ID3D11VertexShader>	vertex_shader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>	pixel_shader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>	input_layout;

	Microsoft::WRL::ComPtr<ID3D11BlendState>		blend_state;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState>	rasterizer_state;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState>	depth_stencil_state;

	std::vector<Sphere>		spheres;
	std::vector<Cylinder>	cylinders;

	UINT	sphereVertexCount = 0;
	UINT	sphereCylinderCount = 0;
};