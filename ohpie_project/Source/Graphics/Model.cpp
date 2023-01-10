#include "Model.h"
#include <vector>
#include "Graphics.h"
Model::Model(const char* filename)
{
	
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(filename, 
											aiProcess_Triangulate | 
											aiProcess_ConvertToLeftHanded);
	if (!scene)
	{
		return;
	}
	processNode(scene->mRootNode, scene);

}
void Model::Draw(ID3D11DeviceContext* deviceContext)
{
	for (size_t i = 0; i < meshes.size(); i++)
	{
		meshes[i].Draw(deviceContext);
	}
}
void Model::processNode(aiNode* node, const aiScene* scene)
{
	for (UINT i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.emplace_back(processMesh(mesh, scene));
	}
	for (UINT i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);
	}
}