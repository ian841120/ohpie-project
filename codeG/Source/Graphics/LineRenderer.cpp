#include "LineRenderer.h"
#include <memory>
LineRenderer::LineRenderer(ID3D11Device* device,UINT VertexCount):MaxVertexCount(VertexCount)
{
	HRESULT hr{ S_OK };
	//Create Vertex Shader and input layout
	{ 
		FILE* fp{};
		fopen_s(&fp, "./Shader/LineRendererVS.cso", "rb");
		_ASSERT_EXPR(fp, L"CSO file not found ");
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
			{"COLOR",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0},
		};
		hr = device->CreateInputLayout(input_element_desc, _countof(input_element_desc), cso_data.get(), cso_sz, input_layout.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), L"Failed to create inputlayout");
	}
	// Create pixel shader
	{
		FILE* fp{};
		fopen_s(&fp, "./Shader/LineRendererPS.cso", "rb");
		_ASSERT_EXPR(fp, L"CSO file not found ");
		fseek(fp, 0,SEEK_END);
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
		buffer_desc.ByteWidth = sizeof(Cbuffer);
		buffer_desc.Usage = D3D11_USAGE_DEFAULT;
		buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		buffer_desc.CPUAccessFlags = 0;
		buffer_desc.MiscFlags = 0;
		buffer_desc.StructureByteStride = 0;
		hr = device->CreateBuffer(&buffer_desc, 0, constant_buffer.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), L"Failed to create constant buffer");
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
	//Create depth stencil buffer
	{
		D3D11_DEPTH_STENCIL_DESC depth_stencil_desc{};
		depth_stencil_desc.DepthEnable = true;
		depth_stencil_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		depth_stencil_desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

		HRESULT hr = device->CreateDepthStencilState(&depth_stencil_desc, depth_stencil_state.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), L"Failed to create depth stencil state");
	}
	//Create rasterizer state
	{
		D3D11_RASTERIZER_DESC rasterize_desc{};
		rasterize_desc.FrontCounterClockwise = true;
		rasterize_desc.DepthBias = 0;
		rasterize_desc.DepthBiasClamp = 0;
		rasterize_desc.SlopeScaledDepthBias = 0;
		rasterize_desc.DepthClipEnable = true;
		rasterize_desc.ScissorEnable = false;
		rasterize_desc.MultisampleEnable = true;
		rasterize_desc.FillMode = D3D11_FILL_SOLID;
		rasterize_desc.CullMode = D3D11_CULL_NONE;
		rasterize_desc.AntialiasedLineEnable = false;

		hr = device->CreateRasterizerState(&rasterize_desc, rasterizer_state.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), L"Failed to create rasterizer state");
	}
	//Create vertex buffer
	{
		D3D11_BUFFER_DESC buffer_desc{};
		buffer_desc.ByteWidth = sizeof(Vertex) * VertexCount;
		buffer_desc.Usage = D3D11_USAGE_DYNAMIC;
		buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		buffer_desc.MiscFlags = 0;
		buffer_desc.StructureByteStride = 0;
		hr=device->CreateBuffer(&buffer_desc, 0, vertex_buffer.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), L"Failed to create vertex buffer");

	}
}
void LineRenderer::Render(RenderContext&rc)
{
	//Set shader and input layout
	rc.deviceContext->VSSetShader(vertex_shader.Get(), nullptr, 0);
	rc.deviceContext->PSSetShader(pixel_shader.Get(), nullptr, 0);
	rc.deviceContext->IASetInputLayout(input_layout.Get());

	//Set constantbuffer
	rc.deviceContext->VSSetConstantBuffers(0, 1, constant_buffer.GetAddressOf());
	//Set render target view
	float blendFactor[4] = { 1.0f,1.0f,1.0f,1.0f };
	rc.deviceContext->OMSetBlendState(blend_state.Get(), blendFactor, 0xFFFFFFFF);
	rc.deviceContext->OMSetDepthStencilState(depth_stencil_state.Get(), 0);
	rc.deviceContext->RSSetState(rasterizer_state.Get());
	// Update constant buffer
	DirectX::XMMATRIX V = DirectX::XMLoadFloat4x4(&rc.view);
	DirectX::XMMATRIX P = DirectX::XMLoadFloat4x4(&rc.projection);
	DirectX::XMMATRIX VP = V * P;
	Cbuffer cbuffer;
	DirectX::XMStoreFloat4x4(&cbuffer.world_view_projection, VP);
	rc.deviceContext->UpdateSubresource(constant_buffer.Get(), 0, 0, &cbuffer, 0, 0);
	UINT stride{ sizeof(Vertex) };
	UINT offset{ 0 };
	rc.deviceContext->IASetVertexBuffers(0, 1, vertex_buffer.GetAddressOf(), &stride, &offset);
	rc.deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	// Draw Vertices
	UINT totalVertexCount = static_cast<UINT>(vertices.size());
	UINT start = 0;
	UINT count = (totalVertexCount < MaxVertexCount) ? totalVertexCount : MaxVertexCount;

	while (start < totalVertexCount)
	{
		D3D11_MAPPED_SUBRESOURCE mapped_subresource{};
		HRESULT hr = rc.deviceContext->Map(vertex_buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_subresource);
		_ASSERT_EXPR(SUCCEEDED(hr), L"Failed to Mapping");

		memcpy(mapped_subresource.pData, &vertices[start], sizeof(Vertex) * count);
		rc.deviceContext->Unmap(vertex_buffer.Get(),0);
		rc.deviceContext->Draw(count, 0);
		start += count;
		if (start + count > totalVertexCount)
		{
			count = totalVertexCount - start;
		}
	}
	vertices.clear();
}
void LineRenderer::AddVertex(const DirectX::XMFLOAT3& position, const DirectX::XMFLOAT4& color)
{
	Vertex vertex;
	vertex.position = position;
	vertex.color = color;
	vertices.emplace_back(vertex);
}