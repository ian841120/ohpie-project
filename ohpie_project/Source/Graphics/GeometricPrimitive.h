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
	void Render(ID3D11DeviceContext* device_context, const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& prjection,const DirectX::XMFLOAT4& lightDirection);
	void DrawPrimitiveCuboid(const DirectX::XMFLOAT3& position, float length, float width, float height,const DirectX::XMFLOAT4& color);
	void DrawPrimitiveCylinder(const DirectX::XMFLOAT3& position, float radius, float height, const DirectX::XMFLOAT4& color);
	void DrawPrimitiveCone(const DirectX::XMFLOAT3& position, float radius, float height, const DirectX::XMFLOAT4& color);
	void DrawPrimitiveSphere(const DirectX::XMFLOAT3& position,float radius,const DirectX::XMFLOAT4& color);
	void DrawPrimitiveCapsule();
private:
	struct Cbuffer
	{
		DirectX::XMFLOAT4X4 world;
		DirectX::XMFLOAT4X4	view_project;
		DirectX::XMFLOAT4	color;
		DirectX::XMFLOAT4	lightDirection;
	};
	struct Vertex
	{
		DirectX::XMFLOAT3	position;
		DirectX::XMFLOAT3	normal;
	};
	struct Cuboid
	{
		DirectX::XMFLOAT4 color;
		DirectX::XMFLOAT3 position;
		float length;
		float width;
		float height;
	};
	struct Cylinder
	{
		DirectX::XMFLOAT4	color;
		DirectX::XMFLOAT3	position;
		float				radius;
		float				height;
	};
	struct Cone
	{
		DirectX::XMFLOAT4	color;
		DirectX::XMFLOAT3	position;
		float				radius;
		float				height;
	};
	struct Sphere
	{
		DirectX::XMFLOAT4	color;
		DirectX::XMFLOAT3	position;
		float				radius;
	};

private:
	void CreatePrimitiveCuboid(ID3D11Device* device, float length, float width, float height);
	void CreatePrimitiveCylinder(ID3D11Device* device, const DirectX::XMFLOAT3& start, float radius1, float radius2, float height, int slices);
	void CreatePrimitiveCone(ID3D11Device* device, const DirectX::XMFLOAT3& start, float radius1, float radius2, float height, int slices);
	void CreatePrimitiveSphere(ID3D11Device* device, const DirectX::XMFLOAT3& center, float radius, int slices, int stacks);
private:
	Microsoft::WRL::ComPtr<ID3D11Buffer>	constant_buffer;

	Microsoft::WRL::ComPtr<ID3D11Buffer>	cuboid_vertex_buffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer>	cuboid_index_buffer;

	Microsoft::WRL::ComPtr<ID3D11Buffer>	cylinder_vertex_buffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer>	cylinder_index_buffer;

	Microsoft::WRL::ComPtr<ID3D11Buffer>	cone_vertex_buffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer>	cone_index_buffer;


	Microsoft::WRL::ComPtr<ID3D11Buffer>	sphere_vertex_buffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer>	sphere_index_buffer;


	Microsoft::WRL::ComPtr<ID3D11VertexShader>	vertex_shader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>	pixel_shader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>	input_layout;

	Microsoft::WRL::ComPtr<ID3D11BlendState>		blend_state;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState>	depth_stencil_state;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState>	rasterizer_state;

	std::vector<Cuboid>cuboids;
	std::vector<Cylinder>cylinders;
	std::vector<Cone>cones;
	std::vector<Sphere>spheres;
	UINT	cuboidIndexCount = 0;
	UINT	cylinderIndexCount = 0;
	UINT	sphereIndexCount = 0;
};