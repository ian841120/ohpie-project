#include "Skybox.h"
#include <DDSTextureLoader.h>
#include <memory>
#include <vector>
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
		FILE* fp{};
		fopen_s(&fp, "./Shader/SkyBoxVS.cso", "rb");
		_ASSERT_EXPR(fp, L"CSO not found");
		fseek(fp, 0, SEEK_END);
		long cso_sz{ ftell(fp) };
		fseek(fp, 0, SEEK_SET);
		std::unique_ptr<unsigned char[]>cso_data = std::make_unique<unsigned char[]>(cso_sz);
		fread(cso_data.get(), cso_sz, 1, fp);
		fclose(fp);
		hr = device->CreateVertexShader(cso_data.get(), cso_sz, nullptr, vertex_shader.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), L"Failed to create vertex shader");

		D3D11_INPUT_ELEMENT_DESC input_element_desc[] =
		{
			{"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0},
		};

		hr = device->CreateInputLayout(input_element_desc, _countof(input_element_desc), cso_data.get(), cso_sz, input_layout.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), L"Failed to create inputlayout");

	}
	//Create Pixel Shader
	{
		FILE* fp{};
		fopen_s(&fp, "./Shader/SkyBoxPS.cso", "rb");
		_ASSERT_EXPR(fp, L"CSO not found");
		fseek(fp, 0, SEEK_END);
		long cso_sz{ ftell(fp) };
		fseek(fp, 0, SEEK_SET);
		std::unique_ptr<unsigned char[]>cso_data = std::make_unique<unsigned char[]>(cso_sz);
		fread(cso_data.get(), cso_sz, 1, fp);
		fclose(fp);
		hr = device->CreatePixelShader(cso_data.get(), cso_sz, nullptr, pixel_shader.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), L"Failed to create pixel shader");
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
	//Create depth stencil state
	{
		D3D11_DEPTH_STENCIL_DESC depth_stencil_desc{};
		depth_stencil_desc.StencilEnable = true;
		depth_stencil_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		depth_stencil_desc.DepthFunc= D3D11_COMPARISON_LESS_EQUAL;
		depth_stencil_desc.StencilEnable = false;
		hr = device->CreateDepthStencilState(&depth_stencil_desc, depth_stencil_state.GetAddressOf());
	}
	//Create sampler state
	{
		D3D11_SAMPLER_DESC sampler_desc{};
		sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		sampler_desc.MipLODBias = 0;
		sampler_desc.MaxAnisotropy = 16;
		sampler_desc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		sampler_desc.BorderColor[0] = 0;
		sampler_desc.BorderColor[1] = 0;
		sampler_desc.BorderColor[2] = 0;
		sampler_desc.BorderColor[3] = 0;
		sampler_desc.MinLOD = 0;
		sampler_desc.MaxLOD = D3D11_FLOAT32_MAX;
		sampler_desc.Filter = D3D11_FILTER_ANISOTROPIC;
		hr = device->CreateSamplerState(&sampler_desc, sampler_state.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), L"Create sampler state failed");
	}
	//Create rasterizer state
	{
		D3D11_RASTERIZER_DESC desc{};
		desc.FrontCounterClockwise = true;
		desc.DepthBias = 0;
		desc.DepthBiasClamp = 0;
		desc.SlopeScaledDepthBias = 0;
		desc.DepthClipEnable = true;
		desc.ScissorEnable = false;
		desc.MultisampleEnable = true;
		desc.FillMode = D3D11_FILL_SOLID;
		desc.CullMode = D3D11_CULL_NONE;
		desc.AntialiasedLineEnable = false;
		hr = device->CreateRasterizerState(&desc, rasterizer_state.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), L"Create rasterizer state failed");

	}
	//Create blend state
	{
		D3D11_BLEND_DESC blend_desc{};
		blend_desc.AlphaToCoverageEnable = false;
		blend_desc.IndependentBlendEnable = false;
		blend_desc.RenderTarget[0].BlendEnable = false;
		blend_desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		blend_desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		blend_desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		blend_desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		blend_desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		blend_desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blend_desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		HRESULT hr = device->CreateBlendState(&blend_desc, blend_state.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), L"Failed to create blend state");
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
	rc.deviceContext->OMSetBlendState(blend_state.Get(), blenderFactor, 0XFFFFFFFF);
	rc.deviceContext->OMSetDepthStencilState(depth_stencil_state.Get(), 0);
	rc.deviceContext->RSSetState(rasterizer_state.Get());

	rc.deviceContext->PSSetShaderResources(0, 1, shader_resource_view.GetAddressOf());
	rc.deviceContext->PSSetSamplers(0, 1, sampler_state.GetAddressOf());
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