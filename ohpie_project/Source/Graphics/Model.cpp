#include "Model.h"
#include "Graphics.h"
#include "Shader.h"
Model::Model(const char* filename)
{
	HRESULT hr;
	ID3D11Device* device = Graphics::GetInstance().GetDevice();

	D3D11_INPUT_ELEMENT_DESC input_element_desc[] =
	{
		{"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0},
		{"NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0},
	};
	
	create_vs_from_file(device, "./Shader/ModelVS.cso", vertex_shader.GetAddressOf(), input_layout.GetAddressOf(), input_element_desc, _countof(input_element_desc));
	create_ps_from_file(device, "./Shader/ModelPS.cso", pixel_shader.GetAddressOf());

	//Create constant buffer
	{
		D3D11_BUFFER_DESC buffer_desc{};
		buffer_desc.ByteWidth = sizeof(constBuffer);
		buffer_desc.Usage = D3D11_USAGE_DEFAULT;
		buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		buffer_desc.CPUAccessFlags = 0;
		buffer_desc.MiscFlags = 0;
		buffer_desc.StructureByteStride = 0;
		hr = device->CreateBuffer(&buffer_desc, 0, constant_buffer.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), L"Failed to create constant buffer");

	}
	//createMesh(device, filename);

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
void Model::Render(RenderContext& rc)
{
	// Set shader
	rc.deviceContext->VSSetShader(vertex_shader.Get(), nullptr, 0);
	rc.deviceContext->PSSetShader(pixel_shader.Get(), nullptr, 0);
	rc.deviceContext->IASetInputLayout(input_layout.Get());

	// Set constant buffer
	rc.deviceContext->VSSetConstantBuffers(0, 1, constant_buffer.GetAddressOf());
	rc.deviceContext->PSSetConstantBuffers(0, 1, constant_buffer.GetAddressOf());

	// Set render target view
	const float blenderFactor[4] = { 1.0f,1.0f,1.0f,1.0f };
	rc.deviceContext->OMSetBlendState(RenderStates::blendStates[static_cast<int>(RenderStates::BS::ALPHA)].Get(), nullptr, 0xFFFFFFFF);
	rc.deviceContext->OMSetDepthStencilState(RenderStates::depthStencilStates[static_cast<int>(RenderStates::DSS::ZT_ON_ZW_ON)].Get(), 0);
	rc.deviceContext->RSSetState(RenderStates::rasterizerStates[static_cast<int>(RenderStates::RS::FILL_WIREFRAME)].Get());

	// Create view projection matrix;
	DirectX::XMMATRIX V = DirectX::XMLoadFloat4x4(&rc.view);
	DirectX::XMMATRIX P = DirectX::XMLoadFloat4x4(&rc.projection);
	DirectX::XMMATRIX VP = V * P;
	// Set Primitive
	UINT stride{ sizeof(Vertex) };
	UINT offset{ 0 };
	rc.deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	for (Mesh& mesh : meshes)
	{
		rc.deviceContext->IASetVertexBuffers(0, 1, mesh.vertex_buffer.GetAddressOf(), &stride, &offset);
		rc.deviceContext->IASetIndexBuffer(mesh.index_buffer.Get(), DXGI_FORMAT_R32_UINT, 0);

		constBuffer cbuffer;
		DirectX::XMStoreFloat4x4(&cbuffer.view_project, VP);
		rc.deviceContext->UpdateSubresource(constant_buffer.Get(), 0, 0, &cbuffer, 0, 0);
		rc.deviceContext->DrawIndexed(static_cast<UINT>(mesh.indices.size()), 0, 0);

	}


}
void Model::processMesh(aiMesh*mesh,const aiScene*scene)
{
	Mesh mesh_;
	for (UINT i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		vertex.position.x = mesh->mVertices[i].x;
		vertex.position.y = mesh->mVertices[i].y;
		vertex.position.z = mesh->mVertices[i].z;

		vertex.normal.x = mesh->mNormals[i].x;
		vertex.normal.y = mesh->mNormals[i].y;
		vertex.normal.z = mesh->mNormals[i].z;

		mesh_.vertices.emplace_back(vertex);
	}

	for (UINT i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (UINT j = 0; j < face.mNumIndices; j++)
		{
			mesh_.indices.emplace_back(face.mIndices[j]);
		}
	}
	ID3D11Device* device = Graphics::GetInstance().GetDevice();
	//Create Vertex buffer
	{
		D3D11_BUFFER_DESC buffer_desc{};
		buffer_desc.ByteWidth = static_cast<UINT>(mesh_.vertices.size() * sizeof(Vertex));
		buffer_desc.Usage = D3D11_USAGE_DEFAULT;
		buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		buffer_desc.CPUAccessFlags = 0;
		buffer_desc.MiscFlags = 0;
		buffer_desc.StructureByteStride = 0;
		D3D11_SUBRESOURCE_DATA subresource_data{};
		subresource_data.pSysMem = mesh_.vertices.data();
		subresource_data.SysMemPitch = 0;
		subresource_data.SysMemSlicePitch = 0;
		HRESULT hr = device->CreateBuffer(&buffer_desc, &subresource_data, mesh_.vertex_buffer.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), L"Failed to vertex buffer ");
		buffer_desc.ByteWidth = static_cast<UINT>(mesh_.indices.size() * sizeof(UINT));
		buffer_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		subresource_data.pSysMem = mesh_.indices.data();
		hr = device->CreateBuffer(&buffer_desc, &subresource_data, mesh_.index_buffer.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), L"Failed to index buffer ");
	}

	meshes.emplace_back(mesh_);
}
void Model::processNode(aiNode* node, const aiScene* scene)
{
	for (UINT i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		processMesh(mesh, scene);
	}
	for (UINT i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);
	}
}
