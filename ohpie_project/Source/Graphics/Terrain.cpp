#include "Terrain.h"
#include "RenderStates.h"
#include "Shader.h"
#include "Graphics.h"

Terrain::Terrain()
{
	ID3D11Device* device = Graphics::GetInstance().GetDevice();
	D3D11_INPUT_ELEMENT_DESC inputElementDesc[] =
	{
		{"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0},
	};
	create_vs_from_file(device, "./Shader/TerrainVS.cso", vertexShader.GetAddressOf(), inputLayout.GetAddressOf(), inputElementDesc, _countof(inputElementDesc));
	create_ps_from_file(device, "./Shader/TerrainPS.cso", pixelShader.GetAddressOf());


	D3D11_BUFFER_DESC desc;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.ByteWidth = sizeof(CBuffer);
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;
	HRESULT hr = device->CreateBuffer(&desc, nullptr, constantBuffer.GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), L"FAIL");
	CreateMesh(width, height);
}
void Terrain::Update(float elapsedTime)
{
	ID3D11DeviceContext* dc = Graphics::GetInstance().GetDeviceContext();
	std::vector<Vertex> temp;
	static float t=0;
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			Vertex vertex;
			vertex.position.x = static_cast<float>(j);

			float e = perlinNoise.OctavePerlin(j / float(width)+ t, i / float(height)+ t, octaves, persistence);
			if(e<0.0)
				vertex.position.y = 0.0f;
			else
				vertex.position.y = e;

			
			vertex.position.z = static_cast<float>(i);
			temp.emplace_back(vertex);
		}

	}
	t += elapsedTime*0.03;
	HRESULT hr{ S_OK };
	//Edit vertex data 
	D3D11_MAPPED_SUBRESOURCE mapped_subresource{};
	hr = dc->Map(vertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_subresource);
	_ASSERT_EXPR(SUCCEEDED(hr), L"Failed to Mapping");
	Vertex* vertices{ reinterpret_cast<Vertex*>(mapped_subresource.pData) };
	if (vertices != nullptr)
	{
		for (int i = 0; i < width; i++)
		{
			for (int j = 0; j < height; j++ )
			{
				vertices[i * width + j].position.x = temp[i * width + j].position.x;
				vertices[i * width + j].position.y = temp[i * width + j].position.y;
				vertices[i * width + j].position.z = temp[i * width + j].position.z;
			}
			
		}
	}
	dc->Unmap(vertexBuffer.Get(), 0);


}
void Terrain::CreateMesh(int width, int height)
{
	ID3D11Device* device = Graphics::GetInstance().GetDevice();

	std::vector<Vertex> vertices;
	std::vector<UINT>	indices;

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			Vertex vertex;
			vertex.position.x = static_cast<float>(j);
			double e = perlinNoise.OctavePerlin(j / float(width), i / float(height), 6, persistence);
			vertex.position.y = static_cast<float>(e);
			vertex.position.z = static_cast<float>(i);
			vertices.emplace_back(vertex);
		}

	}

	for (int i = 0; i < height-1; i++)
	{
		for (int j = 0; j < width-1; j++)
		{
			indices.emplace_back(i * height + j);
			indices.emplace_back(i * height + j + 1);
			indices.emplace_back(i * height + j + width);

			indices.emplace_back(i * height + j + 1);
			indices.emplace_back(i * height + j + width+1);
			indices.emplace_back(i * height + j + width);


		}
	}
	indexCount = static_cast<UINT>(indices.size());

	{
		D3D11_BUFFER_DESC buffer_desc{};
		buffer_desc.ByteWidth = static_cast<UINT>(vertices.size() * sizeof(Vertex));
		buffer_desc.Usage = D3D11_USAGE_DYNAMIC;
		buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;;
		buffer_desc.MiscFlags = 0;
		buffer_desc.StructureByteStride = 0;
		D3D11_SUBRESOURCE_DATA subresource_data{};
		subresource_data.pSysMem = vertices.data();
		subresource_data.SysMemPitch = 0;
		subresource_data.SysMemSlicePitch = 0;
		HRESULT hr = device->CreateBuffer(&buffer_desc, &subresource_data, vertexBuffer.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), L"Failed to sphere vertex buffer ");
		buffer_desc.ByteWidth = static_cast<UINT>(indices.size() * sizeof(UINT));
		buffer_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		subresource_data.pSysMem = indices.data();
		hr = device->CreateBuffer(&buffer_desc, &subresource_data, indexBuffer.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), L"Failed to sphere index buffer ");
	}

}
void Terrain::Render(const RenderContext& rc)
{
	rc.deviceContext->VSSetShader(vertexShader.Get(), nullptr, 0);
	rc.deviceContext->PSSetShader(pixelShader.Get(), nullptr, 0);

	rc.deviceContext->IASetInputLayout(inputLayout.Get());
	ID3D11Buffer* constantBuffers[] =
	{
		constantBuffer.Get(),
	};
	rc.deviceContext->VSSetConstantBuffers(0, ARRAYSIZE(constantBuffers), constantBuffers);
	rc.deviceContext->PSSetConstantBuffers(0, ARRAYSIZE(constantBuffers), constantBuffers);
	const float blend_factor[4] = { 1.0f,1.0f,1.0f,1.0f };
	rc.deviceContext->OMSetBlendState(RenderStates::blendStates[static_cast<int>(RenderStates::BS::ALPHA)].Get(), blend_factor, 0xFFFFFFFF);
	rc.deviceContext->OMSetDepthStencilState(RenderStates::depthStencilStates[static_cast<int>(RenderStates::DSS::ZT_ON_ZW_ON)].Get(), 0);
	rc.deviceContext->RSSetState(RenderStates::rasterizerStates[static_cast<int>(RenderStates::RS::FILL_WIREFRAME)].Get());

	DirectX::XMMATRIX V = DirectX::XMLoadFloat4x4(&rc.view);
	DirectX::XMMATRIX P = DirectX::XMLoadFloat4x4(&rc.projection);
	DirectX::XMMATRIX VP = V * P;

	CBuffer cbuffer;

	DirectX::XMStoreFloat4x4(&cbuffer.viewProjection, VP);
	
	DirectX::XMMATRIX S = DirectX::XMMatrixScaling(10,h, 10);
	DirectX::XMMATRIX W = S;
	DirectX::XMStoreFloat4x4(&cbuffer.world, W);

	rc.deviceContext->UpdateSubresource(constantBuffer.Get(), 0, 0, &cbuffer, 0, 0);

	UINT stride{ sizeof(Vertex) };
	UINT offset{ 0 };
	rc.deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	rc.deviceContext->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);
	rc.deviceContext->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	rc.deviceContext->DrawIndexed(indexCount, 0, 0);

}

void Terrain::DebugGUI()
{
	ImGui::Begin("PerlinNoise");
	ImGui::SliderInt("octaves", &octaves, 1, 30);
	ImGui::SliderFloat("persistence", &persistence, 0.01f, 5.0f);
	ImGui::SliderFloat("Height", &h, 1.0f, 500.0f);
	ImGui::SliderFloat("NANIKA", &nanika, 0.01f, 1.0f);
	ImGui::End();

		
}