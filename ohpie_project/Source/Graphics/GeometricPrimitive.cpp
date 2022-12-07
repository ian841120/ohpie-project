#include "GeometricPrimitive.h"
#include <memory>
GeometricPrimitive::GeometricPrimitive(ID3D11Device* device)
{
	HRESULT hr{ S_OK };
	//Create vertex shader and input layout
	{
		FILE* fp{};
		fopen_s(&fp, "./Shader/GeometricPrimitiveVS.cso", "rb");
		_ASSERT_EXPR(fp, L"CSO File not found");
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
			{"NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0},
		};
		hr = device->CreateInputLayout(input_element_desc, _countof(input_element_desc), cso_data.get(), cso_sz, input_layout.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), L"Failed to create inputlayout");
	}
	//Create pixel shader
	{
		FILE* fp{};
		fopen_s(&fp, "./Shader/GeometricPrimitivePS.cso", "rb");
		_ASSERT_EXPR(fp, L"CSO File not found");
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
	CreatePrimitiveCuboid(device, 1.0f, 1.0f, 1.0f);
	CreatePrimitiveCylinder(device, { 0.0f,0.0f,0.0f }, 1.0f, 1.0f, 1.0f, 16);
}
void GeometricPrimitive::Render(ID3D11DeviceContext* device_context, const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection, const DirectX::XMFLOAT4& lightDirection)
{
	// Set shader
	device_context->VSSetShader(vertex_shader.Get(), nullptr, 0);
	device_context->PSSetShader(pixel_shader.Get(), nullptr, 0);
	device_context->IASetInputLayout(input_layout.Get());

	// Set constant buffer
	device_context->VSSetConstantBuffers(0, 1, constant_buffer.GetAddressOf());

	// Set render target view
	const float blenderFactor[4] = { 1.0f,1.0f,1.0f,1.0f };
	device_context->OMSetBlendState(blend_state.Get(), blenderFactor, 0XFFFFFFFF);
	device_context->OMSetDepthStencilState(depth_stencil_state.Get(), 0);
	device_context->RSSetState(rasterizer_state.Get());

	// Create view projection matrix;
	DirectX::XMMATRIX V = DirectX::XMLoadFloat4x4(&view);
	DirectX::XMMATRIX P = DirectX::XMLoadFloat4x4(&projection);
	DirectX::XMMATRIX VP = V * P;
	// Set Primitive
	UINT stride{ sizeof(Vertex) };
	UINT offset{ 0 };
	device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//Draw cubiod
	device_context->IASetVertexBuffers(0, 1, cuboid_vertex_buffer.GetAddressOf(), &stride, &offset);
	device_context->IASetIndexBuffer(cuboid_index_buffer.Get(), DXGI_FORMAT_R32_UINT, 0);

	for(const Cuboid& cuboid:cuboids)
	{
		DirectX::XMMATRIX S = DirectX::XMMatrixScaling(cuboid.length, cuboid.height, cuboid.width);
		DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(cuboid.position.x, cuboid.position.y, cuboid.position.z);
		DirectX::XMMATRIX W = S * T;
		Cbuffer cbuffer;
		DirectX::XMStoreFloat4x4(&cbuffer.view_project, VP);
		DirectX::XMStoreFloat4x4(&cbuffer.world, W);
		cbuffer.lightDirection = lightDirection;
		cbuffer.color = cuboid.color;
		device_context->UpdateSubresource(constant_buffer.Get(), 0, 0, &cbuffer, 0, 0);
		device_context->DrawIndexed(cuboidIndexCount, 0,0);

	}
	cuboids.clear();
	//Draw cylinder

	device_context->IASetVertexBuffers(0, 1, cylinder_vertex_buffer.GetAddressOf(), &stride, &offset);
	device_context->IASetIndexBuffer(cylinder_index_buffer.Get(), DXGI_FORMAT_R32_UINT, 0);

	for (const Cylinder& cylinder : cylinders)
	{
		DirectX::XMMATRIX S = DirectX::XMMatrixScaling(cylinder.radius, cylinder.height, cylinder.radius);
		DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(cylinder.position.x, cylinder.position.y, cylinder.position.z);
		DirectX::XMMATRIX W = S * T;
		Cbuffer cbuffer;
		DirectX::XMStoreFloat4x4(&cbuffer.view_project, VP);
		DirectX::XMStoreFloat4x4(&cbuffer.world, W);
		cbuffer.lightDirection = lightDirection;
		cbuffer.color = cylinder.color;
		device_context->UpdateSubresource(constant_buffer.Get(), 0, 0, &cbuffer, 0, 0);
		device_context->DrawIndexed(cylinderIndexCount, 0, 0);

	}
	cylinders.clear();
}
void GeometricPrimitive::CreatePrimitiveCuboid(ID3D11Device* device, float length, float width, float height)
{
	DirectX::XMFLOAT3 position{ length / 2,height / 2,width / 2 };
	Vertex vertices[24]{};
	uint32_t indices[36]{};
	cuboidIndexCount = sizeof(indices) / sizeof(uint32_t);
	// Front
	//
	// 0--------1
	// |		|
	// |   -Z	|
	// |		|
	// 2--------3
	int face = 0;
	vertices[face * 4 + 0].position = {-position.x,+position.y,-position.z};
	vertices[face * 4 + 1].position = {+position.x,+position.y,-position.z};
	vertices[face * 4 + 2].position = {-position.x,-position.y,-position.z};
	vertices[face * 4 + 3].position = {+position.x,-position.y,-position.z};
	vertices[face * 4 + 0].normal = { +0.0f,+0.0f,-1.0f };
	vertices[face * 4 + 1].normal = { +0.0f,+0.0f,-1.0f };
	vertices[face * 4 + 2].normal = { +0.0f,+0.0f,-1.0f };
	vertices[face * 4 + 3].normal = { +0.0f,+0.0f,-1.0f };
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
	vertices[face * 4 + 0].normal = { +0.0f,+1.0f,+0.0f };
	vertices[face * 4 + 1].normal = { +0.0f,+1.0f,+0.0f };
	vertices[face * 4 + 2].normal = { +0.0f,+1.0f,+0.0f };
	vertices[face * 4 + 3].normal = { +0.0f,+1.0f,+0.0f };
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
	vertices[face * 4 + 0].normal = { +1.0f,+0.0f,+0.0f };
	vertices[face * 4 + 1].normal = { +1.0f,+0.0f,+0.0f };
	vertices[face * 4 + 2].normal = { +1.0f,+0.0f,+0.0f };
	vertices[face * 4 + 3].normal = { +1.0f,+0.0f,+0.0f };
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
	vertices[face * 4 + 0].normal = { +0.0f,+0.0f,+1.0f };
	vertices[face * 4 + 1].normal = { +0.0f,+0.0f,+1.0f };
	vertices[face * 4 + 2].normal = { +0.0f,+0.0f,+1.0f };
	vertices[face * 4 + 3].normal = { +0.0f,+0.0f,+1.0f };
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
	vertices[face * 4 + 0].normal = { +0.0f,-1.0f,+0.0f };
	vertices[face * 4 + 1].normal = { +0.0f,-1.0f,+0.0f };
	vertices[face * 4 + 2].normal = { +0.0f,-1.0f,+0.0f };
	vertices[face * 4 + 3].normal = { +0.0f,-1.0f,+0.0f };
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
	vertices[face * 4 + 0].normal = { -1.0f,+0.0f,+0.0f };
	vertices[face * 4 + 1].normal = { -1.0f,+0.0f,+0.0f };
	vertices[face * 4 + 2].normal = { -1.0f,+0.0f,+0.0f };
	vertices[face * 4 + 3].normal = { -1.0f,+0.0f,+0.0f };
	indices[face * 6 + 0] = face * 4 + 0;
	indices[face * 6 + 1] = face * 4 + 2;
	indices[face * 6 + 2] = face * 4 + 1;
	indices[face * 6 + 3] = face * 4 + 1;
	indices[face * 6 + 4] = face * 4 + 2;
	indices[face * 6 + 5] = face * 4 + 3;

	// Create Vertex Buffer and index buffer
	{
		// Vertex buffer
		D3D11_BUFFER_DESC buffer_desc{};
		buffer_desc.ByteWidth = static_cast<UINT>(sizeof(vertices));
		buffer_desc.Usage = D3D11_USAGE_DEFAULT;
		buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		buffer_desc.CPUAccessFlags = 0;
		buffer_desc.MiscFlags = 0;
		buffer_desc.StructureByteStride = 0;
		D3D11_SUBRESOURCE_DATA subresource_data{};
		subresource_data.pSysMem = vertices;
		subresource_data.SysMemPitch = 0;
		subresource_data.SysMemSlicePitch = 0;
		HRESULT hr = device->CreateBuffer(&buffer_desc, &subresource_data, cuboid_vertex_buffer.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), L"Failed to create cuboid vertex buffer ");
		// Index buffer
		buffer_desc.ByteWidth = static_cast<UINT>(sizeof(indices));
		buffer_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		subresource_data.pSysMem = indices;
		hr = device->CreateBuffer(&buffer_desc, &subresource_data, cuboid_index_buffer.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), L"Failed create cuboid index buffer ");

	}
}
void GeometricPrimitive::CreatePrimitiveCylinder(ID3D11Device* device, const DirectX::XMFLOAT3& start, float radius1, float radius2, float height, int slices)
{
	std::vector<Vertex>vertices;
	std::vector<UINT>indices;
	Vertex vertex;
	float thetaStep = DirectX::XM_2PI / slices;
	// Create bottom center
	vertex.position = start;
	vertex.normal = { 0.0f,-1.0f,0.0f };
	vertices.emplace_back(vertex);
	// Create bottom vertex
	for (int i = 0; i < slices; i++)
	{
		vertex.position = { radius1 * cosf(i * thetaStep),start.y,radius1 * sinf(i * thetaStep) };
		vertex.normal = { 0.0f,-1.0f,0.0f };
		vertices.emplace_back(vertex);
	}
	// Create top center
	vertex.position = { start.x,start.y + height,start.z };
	vertex.normal = { 0.0f,1.0f,0.0f };
	vertices.emplace_back(vertex);
	// Create top vertex 
	for (int i = 0; i < slices; i++)
	{
		vertex.position = { radius2 * cosf(i * thetaStep),start.y + height,radius2 * sinf(i * thetaStep) };
		vertex.normal = { 0.0f,1.0f,0.0f };
		vertices.emplace_back(vertex);
	}
	// Create bottom side vertex
	for (int i = 0; i < slices; i++)
	{
		vertex.position = { radius1 * cosf(i * thetaStep),start.y,radius1 * sinf(i * thetaStep) };
		vertex.normal = { cosf(i * thetaStep),0.0f,sinf(i * thetaStep) };
		vertices.emplace_back(vertex);
	}
	// Create top side vertex 
	for (int i = 0; i < slices; i++)
	{
		vertex.position = { radius1 * cosf(i * thetaStep),start.y + height,radius1 * sinf(i * thetaStep) };
		vertex.normal = { cosf(i * thetaStep),0.0f,sinf(i * thetaStep) };
		vertices.emplace_back(vertex);
	}

	// Bottom index
	int count = 0;
	for (int i = 0; i < (slices-1)*3;i+=3 )
	{
		indices.emplace_back(static_cast<UINT>(count + i % 3));
		indices.emplace_back(static_cast<UINT>(count + i / 3 + 1));
		indices.emplace_back(static_cast<UINT>(count + i / 3 + 2));
	}
	indices.emplace_back(static_cast<UINT>(count + 0));
	indices.emplace_back(static_cast<UINT>(count + slices));
	indices.emplace_back(static_cast<UINT>(count + 1));
	count += slices + 1;
	// Top index
	for (int i = 0; i < (slices - 1) * 3; i += 3)
	{
		indices.emplace_back(static_cast<UINT>(count + i % 3));
		indices.emplace_back(static_cast<UINT>(count + i / 3 + 2));
		indices.emplace_back(static_cast<UINT>(count + i / 3 + 1));
	}
	indices.emplace_back(static_cast<UINT>(count + 0));
	indices.emplace_back(static_cast<UINT>(count + 1));
	indices.emplace_back(static_cast<UINT>(count + slices));
	// Side index
	count += slices + 1;
	for (int i = 0; i < (slices - 1) * 3; i += 3)
	{
		indices.emplace_back(static_cast<UINT>(count + i / 3));
		indices.emplace_back(static_cast<UINT>(count + i / 3 + slices));
		indices.emplace_back(static_cast<UINT>(count + i / 3 +1 ));
	}
	indices.emplace_back(static_cast<UINT>(count + slices-1));
	indices.emplace_back(static_cast<UINT>(count + 0));
	indices.emplace_back(static_cast<UINT>(count + slices+slices-1));

	count += slices;
	for (int i = 0; i < (slices - 1) * 3; i += 3)
	{
		indices.emplace_back(static_cast<UINT>(count + i / 3));
		indices.emplace_back(static_cast<UINT>(count + i / 3 + 1));
		indices.emplace_back(static_cast<UINT>(count + i / 3 - slices + 1));
	}
	indices.emplace_back(static_cast<UINT>(count + slices - 1));
	indices.emplace_back(static_cast<UINT>(count + 0));
	indices.emplace_back(static_cast<UINT>(count - slices));

	cylinderIndexCount = static_cast<UINT>(indices.size());
	//Create Vertex buffer
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
		HRESULT hr = device->CreateBuffer(&buffer_desc, &subresource_data, cylinder_vertex_buffer.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), L"Failed to cylinder vertex buffer ");
		buffer_desc.ByteWidth = static_cast<UINT>(indices.size() * sizeof(UINT));
		buffer_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		subresource_data.pSysMem = indices.data();
		hr = device->CreateBuffer(&buffer_desc, &subresource_data, cylinder_index_buffer.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), L"Failed to cylinder index buffer ");
	}
}
void GeometricPrimitive::DrawPrimitiveCuboid(const DirectX::XMFLOAT3& position, float length, float height, float width, const DirectX::XMFLOAT4& color)
{
	Cuboid cuboid;
	cuboid.position = position;
	cuboid.length = length;
	cuboid.width = width;
	cuboid.height = height;
	cuboid.color = color;
	cuboids.emplace_back(cuboid);
}
void GeometricPrimitive::DrawPrimitiveCylinder(const DirectX::XMFLOAT3& position, float radius, float height, const DirectX::XMFLOAT4& color)
{
	Cylinder cylinder;
	cylinder.position = position;
	cylinder.height = height;
	cylinder.radius = radius;
	cylinder.color = color;
	cylinders.emplace_back(cylinder);
}