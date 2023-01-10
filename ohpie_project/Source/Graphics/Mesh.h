#pragma once
#include <directxmath.h>
#include <wrl.h>
#include <string>
#include <d3d11.h>
#include <vector>
struct Vertex
{
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT2 texcoord;
};
struct Texture
{
	std::string type;
	std::string	path;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>texture;
};
class Mesh
{
public:
	std::vector<Vertex>vertices;
	std::vector<UINT>indices;
	std::vector<Texture>textures;
	Microsoft::WRL::ComPtr<ID3D11Device>device;
	Mesh(ID3D11Device* device, const std::vector<Vertex>& vertices, const std::vector<UINT>& indices, const std::vector<Texture>& textures);
	void Draw(ID3D11DeviceContext* deviceContext);
private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> VertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> IndexBuffer;
	void setupMesh(ID3D11Device* device);
};