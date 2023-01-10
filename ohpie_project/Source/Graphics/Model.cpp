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

	std::vector<Vertex>vertices;
	std::vector<UINT>indices;

	if (scene->HasMeshes())
	{
		for (unsigned int i = 0; i < scene->mNumMeshes; i++)
		{
			const auto mesh = scene->mMeshes[i];
			for (unsigned int j = 0; j < mesh->mNumVertices; j++)
			{

			}
		}

	}
}