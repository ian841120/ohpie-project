#include "Skybox.h"
#include <DDSTextureLoader.h>
#include <vector>
#include "Shader.h"
#include "RenderStates.h"
Skybox::Skybox(ID3D11Device* device)
{
	HRESULT hr{ S_OK };
	// Create vertex buffer
	{
		std::vector<Vertex> vertices;
		std::vector<UINT>indices;
		DirectX::XMFLOAT3 center = { 0.0f,0.0f,0.0f };
		float radius = 5;
		int slices = 20;
		int stacks = 20;

		Vertex vertex;
		// top position
		vertex.position = { center.x,center.y + radius,center.z };
		vertices.emplace_back(vertex);
		float thetaStep = DirectX::XM_2PI / slices;
		float phiStep = DirectX::XM_PI / stacks;
		// vertex
		for (int i = 1; i <= stacks - 1; i++)
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
		// bottom position	
		vertex.position = { center.x,center.y - radius,center.z };
		vertices.emplace_back(vertex);
		// Top index
		for (int i = 1; i <= slices; i++)
		{
			indices.emplace_back(0);
			indices.emplace_back(i);
			indices.emplace_back(i + 1);
		}
		// Side index
		UINT start = 1;

		for (int i = 0; i < stacks - 2; i++)
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
			start += static_cast<UINT>(stacks + 1);

		}

		// Bottom index
		for (int i = 1; i <= slices; i++)
		{
			indices.emplace_back((stacks - 1) * (slices + 1) + 1);
			indices.emplace_back((stacks - 2) * (slices + 1) + i + 1);
			indices.emplace_back((stacks - 2) * (slices + 1) + i);

		}
		sphereIndexCount = static_cast<UINT>(indices.size());

		// Create Vertex Buffer and index buffer
		{
			// Vertex buffer
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
			HRESULT hr = device->CreateBuffer(&buffer_desc, &subresource_data, vertex_buffer.GetAddressOf());
			_ASSERT_EXPR(SUCCEEDED(hr), L"Failed to create cuboid vertex buffer ");
			// Index buffer
			buffer_desc.ByteWidth = static_cast<UINT>(indices.size()*sizeof(UINT));
			buffer_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
			subresource_data.pSysMem = indices.data();
			hr = device->CreateBuffer(&buffer_desc, &subresource_data, index_buffer.GetAddressOf());
			_ASSERT_EXPR(SUCCEEDED(hr), L"Failed create cuboid index buffer ");

		}

	}
	//Create vertex shader
	{

		D3D11_INPUT_ELEMENT_DESC input_element_desc[] =
		{
			{"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0},
		};

		create_vs_from_file(device, "./Shader/SkyBoxVS.cso", vertex_shader.GetAddressOf(), input_layout.GetAddressOf(), input_element_desc, _countof(input_element_desc));
	}
	//Create Pixel Shader
	{
		create_ps_from_file(device, "./Shader/SkyBoxPS.cso", pixel_shader.GetAddressOf());
	}
	//Create constant buffer
	{
		D3D11_BUFFER_DESC buffer_desc{};
		buffer_desc.Usage = D3D11_USAGE_DEFAULT;
		buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		buffer_desc.CPUAccessFlags = 0;
		buffer_desc.MiscFlags = 0;
		buffer_desc.ByteWidth = sizeof(Cbuffer);
		buffer_desc.StructureByteStride = 0;

		hr = device->CreateBuffer(&buffer_desc, 0, constant_buffer.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), L"Failed to create constant buffer");
	}
	//Create shader resource view
	{
		Microsoft::WRL::ComPtr<ID3D11Resource>resource;
		hr = DirectX::CreateDDSTextureFromFile(device, L"./Data/Skybox/skybox2.dds", resource.GetAddressOf(), shader_resource_view.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), L"Failed to shader resource view");
		
		
	}
}
void Skybox::Render(const RenderContext& rc)
{
	//Set shader
	rc.deviceContext->VSSetShader(vertex_shader.Get(), nullptr, 0);
	rc.deviceContext->PSSetShader(pixel_shader.Get(), nullptr, 0);
	rc.deviceContext->IASetInputLayout(input_layout.Get());


	rc.deviceContext->VSSetConstantBuffers(0, 1, constant_buffer.GetAddressOf());
	//Create view projection
	{
		DirectX::XMMATRIX W = DirectX::XMMatrixIdentity();
		DirectX::XMMATRIX S = DirectX::XMMatrixScaling(1.0f, 1.0f, 1.0f);
		DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(rc.viewPosition.x, rc.viewPosition.y, rc.viewPosition.z);
		W = S * T;
		DirectX::XMMATRIX V = DirectX::XMLoadFloat4x4(&rc.view);
		DirectX::XMMATRIX P = DirectX::XMLoadFloat4x4(&rc.projection);
		DirectX::XMMATRIX VP = V * P;
		Cbuffer cbuffer{};
		DirectX::XMStoreFloat4x4(&cbuffer.view_projeciton,VP);
		DirectX::XMStoreFloat4x4(&cbuffer.world, W);

		rc.deviceContext->UpdateSubresource(constant_buffer.Get(), 0, nullptr, &cbuffer, 0, 0);

	}
	UINT stride{ sizeof(Vertex) };
	UINT offset{ 0 };
	rc.deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	rc.deviceContext->IASetVertexBuffers(0, 1, vertex_buffer.GetAddressOf(), &stride, &offset);
	rc.deviceContext->IASetIndexBuffer(index_buffer.Get(), DXGI_FORMAT_R32_UINT, 0);

	//Set target render view;
	const float blenderFactor[4] = { 1.0f,1.0f,1.0f,1.0f };
	rc.deviceContext->OMSetBlendState(RenderStates::blendStates[static_cast<int>(RenderStates::BS::NONE)].Get(), blenderFactor, 0XFFFFFFFF);
	rc.deviceContext->OMSetDepthStencilState(RenderStates::depthStencilStates[static_cast<int>(RenderStates::DSS::ZT_OFF_ZW_OFF)].Get(), 0);
	rc.deviceContext->RSSetState(RenderStates::rasterizerStates[static_cast<int>(RenderStates::RS::FILL_SOLID)].Get());

	rc.deviceContext->PSSetShaderResources(0, 1, shader_resource_view.GetAddressOf());
	rc.deviceContext->PSSetSamplers(0, 1, RenderStates::samplerStates[static_cast<int>(RenderStates::SS::ANISOTROPIC_SAMPLER_STATE)].GetAddressOf());
	rc.deviceContext->DrawIndexed(sphereIndexCount, 0, 0);
}
// TODO 2 : put in
/*DirectX::XMFLOAT3 position{ 0.5f,0.5f,0.5f };
		Vertex vertices[24]{};
		uint32_t indices[36]{};
		int face = 0;
		vertices[face * 4 + 0].position = { -position.x,+position.y,-position.z };
		vertices[face * 4 + 1].position = { +position.x,+position.y,-position.z };
		vertices[face * 4 + 2].position = { -position.x,-position.y,-position.z };
		vertices[face * 4 + 3].position = { +position.x,-position.y,-position.z };
		indices[face * 6 + 0] = face * 4 + 0;
		indices[face * 6 + 1] = face * 4 + 1;
		indices[face * 6 + 2] = face * 4 + 2;
		indices[face * 6 + 3] = face * 4 + 1;
		indices[face * 6 + 4] = face * 4 + 3;
		indices[face * 6 + 5] = face * 4 + 2;
		// Top
		//
		// 0--------1
		// |		|
		// |   +Y	|
		// |		|
		// 2--------3
		face++;
		vertices[face * 4 + 0].position = { -position.x,+position.y,+position.z };
		vertices[face * 4 + 1].position = { +position.x,+position.y,+position.z };
		vertices[face * 4 + 2].position = { -position.x,+position.y,-position.z };
		vertices[face * 4 + 3].position = { +position.x,+position.y,-position.z };
		indices[face * 6 + 0] = face * 4 + 0;
		indices[face * 6 + 1] = face * 4 + 1;
		indices[face * 6 + 2] = face * 4 + 2;
		indices[face * 6 + 3] = face * 4 + 1;
		indices[face * 6 + 4] = face * 4 + 3;
		indices[face * 6 + 5] = face * 4 + 2;
		// Right
		//
		// 0--------1
		// |		|
		// |   +X	|
		// |		|
		// 2--------3
		face++;
		vertices[face * 4 + 0].position = { +position.x,+position.y,-position.z };
		vertices[face * 4 + 1].position = { +position.x,+position.y,+position.z };
		vertices[face * 4 + 2].position = { +position.x,-position.y,-position.z };
		vertices[face * 4 + 3].position = { +position.x,-position.y,+position.z };
		indices[face * 6 + 0] = face * 4 + 0;
		indices[face * 6 + 1] = face * 4 + 1;
		indices[face * 6 + 2] = face * 4 + 2;
		indices[face * 6 + 3] = face * 4 + 1;
		indices[face * 6 + 4] = face * 4 + 3;
		indices[face * 6 + 5] = face * 4 + 2;
		// Back
		//
		// 0--------1   
		// |		|	
		// |   +Z	|
		// |		|
		// 2--------3
		face++;
		vertices[face * 4 + 0].position = { -position.x,+position.y,+position.z };
		vertices[face * 4 + 1].position = { +position.x,+position.y,+position.z };
		vertices[face * 4 + 2].position = { -position.x,-position.y,+position.z };
		vertices[face * 4 + 3].position = { +position.x,-position.y,+position.z };
		indices[face * 6 + 0] = face * 4 + 0;
		indices[face * 6 + 1] = face * 4 + 2;
		indices[face * 6 + 2] = face * 4 + 1;
		indices[face * 6 + 3] = face * 4 + 1;
		indices[face * 6 + 4] = face * 4 + 2;
		indices[face * 6 + 5] = face * 4 + 3;
		// bottom
		//
		// 0--------1   
		// |		|	
		// |   -Y	|
		// |		|
		// 2--------3
		face++;
		vertices[face * 4 + 0].position = { -position.x,-position.y,+position.z };
		vertices[face * 4 + 1].position = { +position.x,-position.y,+position.z };
		vertices[face * 4 + 2].position = { -position.x,-position.y,-position.z };
		vertices[face * 4 + 3].position = { +position.x,-position.y,-position.z };
		indices[face * 6 + 0] = face * 4 + 0;
		indices[face * 6 + 1] = face * 4 + 2;
		indices[face * 6 + 2] = face * 4 + 1;
		indices[face * 6 + 3] = face * 4 + 1;
		indices[face * 6 + 4] = face * 4 + 2;
		indices[face * 6 + 5] = face * 4 + 3;
		// Left
		//
		// 0--------1   
		// |		|	
		// |   -X	|
		// |		|
		// 2--------3
		face++;
		vertices[face * 4 + 0].position = { -position.x,+position.y,-position.z };
		vertices[face * 4 + 1].position = { -position.x,+position.y,+position.z };
		vertices[face * 4 + 2].position = { -position.x,-position.y,-position.z };
		vertices[face * 4 + 3].position = { -position.x,-position.y,+position.z };
		indices[face * 6 + 0] = face * 4 + 0;
		indices[face * 6 + 1] = face * 4 + 2;
		indices[face * 6 + 2] = face * 4 + 1;
		indices[face * 6 + 3] = face * 4 + 1;
		indices[face * 6 + 4] = face * 4 + 2;
		indices[face * 6 + 5] = face * 4 + 3;
*/