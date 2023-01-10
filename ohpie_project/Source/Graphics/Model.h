#pragma once
#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>
#include <directxmath.h>
class Model
{
	Model(const char* filename);
	~Model() {};
	void Draw();
private:
	int vertexCount = 0;
	int indexCount = 0;

	struct Vertex
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT2 texcoord;
		DirectX::XMFLOAT3 normal;
	};
};