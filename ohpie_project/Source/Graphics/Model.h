#pragma once
#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>

#include "Mesh.h"
class Model
{
	Model(const char* filename);
	~Model() {};
	void Draw(ID3D11DeviceContext*deviceContext);
private:
	std::vector<Mesh>meshes;
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
};