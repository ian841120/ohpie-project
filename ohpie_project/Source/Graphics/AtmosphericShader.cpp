#include "AtmosphericShader.h"
#include "Shader.h"
#include "Graphics.h"
#include "RenderStates.h"
AtmosphericShader::AtmosphericShader()
{
	ID3D11Device* device = Graphics::GetInstance().GetDevice();
	D3D11_INPUT_ELEMENT_DESC inputElementDesc[] =
	{
		{"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0},
	};
	create_vs_from_file(device, "./Shader/AtmosphericShaderVS.cso", vertexShader.GetAddressOf(), inputLayout.GetAddressOf(), inputElementDesc, _countof(inputElementDesc));
	create_ps_from_file(device, "./Shader/AtmosphericShaderPS.cso", pixelShader.GetAddressOf());

	D3D11_BUFFER_DESC desc;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.ByteWidth = sizeof(CbSky);
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;
	HRESULT hr = device->CreateBuffer(&desc, nullptr, skyCbuffer.GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), L"FAIL");


	createDome({ 0.0f,0.0f,0.0f }, 10.0f, 16, 16);

}
void AtmosphericShader::Render(const RenderContext& rc)
{

	rc.deviceContext->VSSetShader(vertexShader.Get(), nullptr, 0);
	rc.deviceContext->PSSetShader(pixelShader.Get(), nullptr, 0);

	rc.deviceContext->IASetInputLayout(inputLayout.Get());
	ID3D11Buffer* constantBuffers[] =
	{
		skyCbuffer.Get(),
	};
	rc.deviceContext->VSSetConstantBuffers(0, ARRAYSIZE(constantBuffers), constantBuffers);
	rc.deviceContext->PSSetConstantBuffers(0, ARRAYSIZE(constantBuffers), constantBuffers);
	const float blend_factor[4] = { 1.0f,1.0f,1.0f,1.0f };
	rc.deviceContext->OMSetBlendState(RenderStates::blendStates[static_cast<int>(RenderStates::BS::ALPHA)].Get(), blend_factor, 0xFFFFFFFF);
	rc.deviceContext->OMSetDepthStencilState(RenderStates::depthStencilStates[static_cast<int>(RenderStates::DSS::ZT_ON_ZW_ON)].Get(), 0);
	rc.deviceContext->RSSetState(RenderStates::rasterizerStates[static_cast<int>(RenderStates::RS::FILL_SOLID)].Get());



	UINT stride{ sizeof(Vertex) };
	UINT offset{ 0 };
	rc.deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	rc.deviceContext->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);
	rc.deviceContext->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	rc.deviceContext->DrawIndexed(indexCount, 0, 0);

}
void AtmosphericShader::createDome(const DirectX::XMFLOAT3& startPosition, float radius, int slices, int stacks)
{
	ID3D11Device* device = Graphics::GetInstance().GetDevice();
	std::vector<Vertex>vertices;
	std::vector<UINT>indices;
	Vertex vertex;
	vertex.position = { startPosition.x,startPosition.y + radius,startPosition.z };
	vertices.emplace_back(vertex);
	float thetaStep = DirectX::XM_2PI / slices;
	float phiStep = DirectX::XM_PIDIV2 / stacks;
	for (int i = 1; i <= stacks; i++)
	{
		float phi = i * phiStep;
		for (int j = 0; j <= slices; j++)
		{
			float theta = j * thetaStep;
			float x = radius * sinf(phi) * cosf(theta);
			float y = radius * cosf(phi);
			float z = radius * sinf(phi) * sinf(theta);
			vertex.position = { x,y,z };
			vertices.emplace_back(vertex);

		}
	}

	// Top index
	for (int i = 1; i <= slices; i++)
	{
		indices.emplace_back(0);
		indices.emplace_back(i);
		indices.emplace_back(i + 1);
	}
	// Side index
	UINT start = 1;
	for (int i = 0; i < stacks - 1; i++)
	{
		for (int j = 0; j < slices; j++)
		{
			indices.emplace_back(j + start);
			indices.emplace_back(j + start + (slices + 1));
			indices.emplace_back(j + start + (slices + 1) + 1);

			indices.emplace_back(j + start);
			indices.emplace_back(j + start + (slices + 1) + 1);
			indices.emplace_back(j + start + 1);

		}

		start += slices + 1;
	}

	indexCount = static_cast<UINT>(indices.size());
	//Create vertex buffer
	{
		D3D11_BUFFER_DESC buffer_desc{};
		buffer_desc.ByteWidth = static_cast<UINT>(vertices.size() * sizeof(Vertex));
		buffer_desc.Usage = D3D11_USAGE_DEFAULT;
		buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		buffer_desc.CPUAccessFlags = 0;
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