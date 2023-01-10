#include "Mesh.h"
Mesh::Mesh(ID3D11Device* device, const std::vector<Vertex>& vertices, const std::vector<UINT>& indices, const std::vector<Texture>& textures):
	vertices(vertices),indices(indices),textures(textures),device(device),VertexBuffer(nullptr),IndexBuffer(nullptr)
{
	setupMesh(device);
}
void Mesh::Draw(ID3D11DeviceContext* deviceContext)
{
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	deviceContext->IASetVertexBuffers(0, 1, VertexBuffer.GetAddressOf(), &stride, &offset);
	deviceContext->IASetIndexBuffer(IndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

	deviceContext->PSSetShaderResources(0, 1, &textures[0].texture);
	deviceContext->DrawIndexed(static_cast<UINT>(indices.size()), 0, 0);
}
void Mesh::setupMesh(ID3D11Device* device)
{
	HRESULT hr;
	D3D11_BUFFER_DESC vertex_buffer_desc;
	vertex_buffer_desc.Usage = D3D11_USAGE_IMMUTABLE;
	vertex_buffer_desc.ByteWidth = static_cast<UINT>(sizeof(Vertex) * vertices.size());
	vertex_buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertex_buffer_desc.CPUAccessFlags = 0;
	vertex_buffer_desc.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA initData;
	initData.pSysMem = &vertices[0];
	hr = device->CreateBuffer(&vertex_buffer_desc, &initData, VertexBuffer.GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), L"Failed to create vertex buffer ");

	D3D11_BUFFER_DESC index_buffer_desc;
	index_buffer_desc.Usage = D3D11_USAGE_IMMUTABLE;
	index_buffer_desc.ByteWidth = (sizeof(UINT) * indices.size());
	index_buffer_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	index_buffer_desc.CPUAccessFlags = 0;
	index_buffer_desc.MiscFlags = 0;

	initData.pSysMem = &indices[0];
	hr = device->CreateBuffer(&index_buffer_desc, &initData, IndexBuffer.GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), L"Failed to create index buffer ");

}
