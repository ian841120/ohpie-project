#pragma once
#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>
#include "RenderContext.h"
#include <vector>
#include <wrl.h>

class Model
{
public:
	Model(const char* filename);
	~Model() {};
	void Render(RenderContext& rc);

	struct Vertex
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT3 normal;
	};

private:

	struct Mesh
	{
		std::vector<Vertex> vertices;
		std::vector<UINT> indices;

		Microsoft::WRL::ComPtr<ID3D11Buffer>	vertex_buffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer>	index_buffer;

	};
	
	struct constBuffer
	{
		DirectX::XMFLOAT4X4 view_project;
	};
private:
	Microsoft::WRL::ComPtr<ID3D11Buffer>	constant_buffer;

	Microsoft::WRL::ComPtr<ID3D11VertexShader>	vertex_shader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>	pixel_shader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>	input_layout;

	
private:
	std::vector<Mesh>meshes;
	void processMesh(aiMesh* mesh, const aiScene* scene);
	void processNode(aiNode* node, const aiScene* scene);
};