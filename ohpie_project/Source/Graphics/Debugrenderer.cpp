#include "DebugRenderer.h"
#include <memory>
DebugRenderer::DebugRenderer(ID3D11Device* device)
{
	HRESULT hr{ S_OK };
	//Create Vertex_shader and input layout
	{
		FILE* fp{};
		fopen_s(&fp, "./Shader/DebugRendererVS.cso", "rb");
		_ASSERT_EXPR(fp, L"CSO File not found");

		fseek(fp, 0, SEEK_END);
		long cso_sz{ ftell(fp) };
		fseek(fp, 0, SEEK_SET);
		std::unique_ptr<unsigned char[]>cso_data{ std::make_unique<unsigned char[]>(cso_sz) };
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
	//Create Pixel shader
	{
		FILE* fp{};
		fopen_s(&fp, "./Shader/DebugRendererPS.cso", "rb");
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
		buffer_desc.Usage = D3D11_USAGE_DEFAULT;
		buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		buffer_desc.CPUAccessFlags = 0;
		buffer_desc.MiscFlags = 0;
		buffer_desc.ByteWidth = sizeof(CbMesh);
		buffer_desc.StructureByteStride = 0;

		hr = device->CreateBuffer(&buffer_desc, 0, constant_buffer.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), L"Failed to create constant buffer");
	}
	//Create blend state
	{
		D3D11_BLEND_DESC desc{};
		desc.AlphaToCoverageEnable = false;
		desc.IndependentBlendEnable = false;
		desc.RenderTarget[0].BlendEnable = false;
		desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		HRESULT hr = device->CreateBlendState(&desc, blend_state.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), L"Failed to create blend state");
	}
	//Create depth stencil buffer
	{
		D3D11_DEPTH_STENCIL_DESC desc{};
		desc.DepthEnable = true;
		desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

		HRESULT hr = device->CreateDepthStencilState(&desc, depth_stencil_state.GetAddressOf());
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
		_ASSERT_EXPR(SUCCEEDED(hr), L"Create rasterizer state failed");

	}
	CreateSphereMesh(device, 1, 16, 16);
	CreateCylinderMesh(device,1.0f, 1.0f, 0.0f, 1.0f, 16, 1);
	CreateCapsuleMesh(device, 1, 1, 16, 16);
}
void DebugRenderer::Render(ID3D11DeviceContext* device_context, const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection)
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
	// Create view projection matrix
	DirectX::XMMATRIX V = DirectX::XMLoadFloat4x4(&view);
	DirectX::XMMATRIX P = DirectX::XMLoadFloat4x4(&projection);
	DirectX::XMMATRIX VP = V * P;
	// Set primitive
	UINT stride{ sizeof(DirectX::XMFLOAT3) };
	UINT offset{ 0 };
	device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	// Draw cylinder
	device_context->IASetVertexBuffers(0, 1, cylinder_vertex_buffer.GetAddressOf(), &stride, &offset);
	for (const Cylinder& cylinder : cylinders)
	{
		DirectX::XMMATRIX S = DirectX::XMMatrixScaling(cylinder.radius, cylinder.height, cylinder.radius);
		DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(cylinder.position.x, cylinder.position.y, cylinder.position.z);
		DirectX::XMMATRIX W = S * T;
		DirectX::XMMATRIX WVP = W * VP;
		CbMesh cbMesh;
		cbMesh.color = cylinder.color;
		DirectX::XMStoreFloat4x4(&cbMesh.world_view_project, WVP);

		device_context->UpdateSubresource(constant_buffer.Get(), 0, 0, &cbMesh, 0, 0);
		device_context->Draw(cylinderVertexCount, 0);
	}
	cylinders.clear();
	//Draw sphere
	device_context->IASetVertexBuffers(0, 1, sphere_vertex_buffer.GetAddressOf(), &stride, &offset);
	for (const Sphere& sphere : spheres)
	{
		DirectX::XMMATRIX S = DirectX::XMMatrixScaling(sphere.radius, sphere.radius, sphere.radius);
		DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(sphere.center.x, sphere.center.y, sphere.center.z);
		DirectX::XMMATRIX W = S * T;
		DirectX::XMMATRIX WVP = W * VP;
		CbMesh cbmesh;
		cbmesh.color = sphere.color;
		DirectX::XMStoreFloat4x4(&cbmesh.world_view_project, WVP);
		device_context->UpdateSubresource(constant_buffer.Get(), 0, 0, &cbmesh, 0, 0);
		device_context->Draw(sphereVertexCount, 0);
	}
	spheres.clear();
	//Draw capsule
	device_context->IASetVertexBuffers(0, 1, capsule_vertex_buffer.GetAddressOf(), &stride, &offset);

	for (const Capsule& capsule : capsules)
	{
		DirectX::XMMATRIX S = DirectX::XMMatrixScaling(capsule.radius, capsule.radius, capsule.radius);
		//DirectX::XMMATRIX S1 = DirectX::XMMatrixScaling(1, capsule.height, 1);

		DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(capsule.position.x, capsule.position.y+ capsule.height, capsule.position.z);
		DirectX::XMMATRIX W = S* T;
		DirectX::XMMATRIX WVP = W * VP;
		CbMesh cbmesh;
		cbmesh.color = capsule.color;
		DirectX::XMStoreFloat4x4(&cbmesh.world_view_project, WVP);
		device_context->UpdateSubresource(constant_buffer.Get(), 0, 0, &cbmesh, 0, 0);
		device_context->Draw(capsuleVertexCount, 0);
	}
	capsules.clear();
}
void DebugRenderer::CreateCylinderMesh(ID3D11Device* device, float radius1, float radius2, float start, float height, int slices, int stacks)
{
	cylinderVertexCount = 2 * slices * (stacks + 1) + 2 * slices;
	std::unique_ptr<DirectX::XMFLOAT3[]>vertices = std::make_unique<DirectX::XMFLOAT3[]>(cylinderVertexCount);
	DirectX::XMFLOAT3* p = vertices.get();
	// Each height of  stack
	float stackHeight = height / stacks;

	// The difference between near two radius
	float radiusStep = (radius2 - radius1) / stacks;
	// Each radius
	float thetaStep = DirectX::XM_2PI / slices;
	for (int i = 0; i < slices; i++)
	{
		float theta = i * thetaStep;
		//first point angle 
		//float sin1 = sinf(i * theta);
		//float cos1 = cosf(i * theta);

		//Next point angle
		//int n = (i + 1) % slices;
		//float sin2 = sinf(n * theta);
		//float cos2 = cosf(n * theta);

		for (int j = 0; j <= stacks; j++)
		{
			// y coordination
			float y = start + j * stackHeight;
			// each circle radius
			float r = radius1 + j * radiusStep;
			// first point
			p->x = r * cosf(theta);
			p->y = y;
			p->z = r * sinf(theta);
			p++;
			theta += thetaStep;
			// next point
			p->x = r * cosf(theta);
			p->y = y;
			p->z = r * sinf(theta);

			p++;
		}
		p->x = radius1* cosf(theta);
		p->y = start;
		p->z = radius1* sinf(theta);
		p++;
		p->x = radius2* cosf(theta);
		p->y = start + height;
		p->z = radius2 * sinf(theta);
		p++;
	}
	//Create Vertex buffer
	{
		D3D11_BUFFER_DESC buffer_desc{};
		buffer_desc.ByteWidth = sizeof(DirectX::XMFLOAT3)*cylinderVertexCount;
		buffer_desc.Usage = D3D11_USAGE_IMMUTABLE;
		buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		buffer_desc.CPUAccessFlags = 0;
		buffer_desc.MiscFlags = 0;
		buffer_desc.StructureByteStride = 0;
		D3D11_SUBRESOURCE_DATA subresource_data{};
		subresource_data.pSysMem = vertices.get();
		subresource_data.SysMemPitch = 0;
		subresource_data.SysMemSlicePitch = 0;
		HRESULT hr = device->CreateBuffer(&buffer_desc, &subresource_data, cylinder_vertex_buffer.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), L"Create cylinder vertex buffer failed");
	}
}
void DebugRenderer::CreateSphereMesh(ID3D11Device* device, float radius, int slices, int stacks)
{
	sphereVertexCount = 2 * slices * (stacks-1) + 2 * stacks * slices;
	std::unique_ptr<DirectX::XMFLOAT3[]>vertices = std::make_unique<DirectX::XMFLOAT3[]>(sphereVertexCount);
	DirectX::XMFLOAT3* p = vertices.get();
	//Draw Latitude
	float thetaStep = DirectX::XM_2PI / slices;
	float phiStep = DirectX::XM_PI / stacks;
	for (int i = 1; i < stacks; i++)
	{
		
		float phi = i * phiStep;
		float y = radius * cosf(phi);
		float r = radius * sinf(phi);
		for (int j = 0; j < slices; j++)
		{
			float theta = j * thetaStep;
			p->x = r * cosf(theta);
			p->y = y;
			p->z = r * sinf(theta);
			p++;
			theta += thetaStep;
			p->x = r * cosf(theta);
			p->y = y;
			p->z = r * sinf(theta);
			p++;
		}
	}
	//Draw Longitude
	phiStep = DirectX::XM_2PI / stacks;
	for (int i = 0; i < slices; i++)
	{
		float theta = i * thetaStep;
		for (int j =0; j < stacks; j++)
		{
			float phi = j * phiStep;
			p->x = radius * cosf(phi) * sinf(theta);
			p->y = radius * sinf(phi);
			p->z = radius * cosf(phi) * cosf(theta);
			p++;
			phi += phiStep;
			p->x = radius * cosf(phi) * sinf(theta);
			p->y = radius * sinf(phi);
			p->z = radius * cosf(phi) * cosf(theta);
			p++;

		}
	}
	//Create vertex buffer
	{
		D3D11_BUFFER_DESC buffer_desc{};
		buffer_desc.ByteWidth = sizeof(DirectX::XMFLOAT3) * sphereVertexCount;
		buffer_desc.Usage = D3D11_USAGE_IMMUTABLE;
		buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		buffer_desc.CPUAccessFlags = 0;
		buffer_desc.MiscFlags = 0;
		buffer_desc.StructureByteStride = 0;
		D3D11_SUBRESOURCE_DATA subresource_data{};
		subresource_data.pSysMem = vertices.get();
		subresource_data.SysMemPitch = 0;
		subresource_data.SysMemSlicePitch = 0;
		HRESULT hr = device->CreateBuffer(&buffer_desc, &subresource_data, sphere_vertex_buffer.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), L"Create sphere vertex buffer failed");

	}

}
void DebugRenderer::CreateCapsuleMesh(ID3D11Device* device, float radius, float height, int slices, int stacks)
{
	capsuleVertexCount = 2 * slices * stacks + 2 * stacks * slices + 2 * slices * 2;
	std::unique_ptr<DirectX::XMFLOAT3[]>vertices = std::make_unique<DirectX::XMFLOAT3[]>(capsuleVertexCount);
	DirectX::XMFLOAT3* p = vertices.get();
	
	float thetaStep = DirectX::XM_2PI / slices;
	float phiStep = DirectX::XM_PI / stacks;
	for (int i =1; i <= stacks; i++)
	{
		
		float phi = i * phiStep;
		if (i >= 9)
		{
			phi -= phiStep;
		}
		float y = radius * cosf(phi);
		if (i <= stacks/2)
		{
			y += height;
		}
		float r = radius * sinf(phi);
		for (int j = 0; j < slices; j++)
		{
			float theta = j * thetaStep;
			p->x = r * cosf(theta);
			p->y = y;
			p->z = r * sinf(theta);
			p++;
			theta += thetaStep;
			p->x = r * cosf(theta);
			p->y = y;
			p->z = r * sinf(theta);
			p++;
		}
	}
	
	//phiStep = DirectX::XM_2PI / stacks;
	//for (int i = 0; i < slices; i++)
	//{
	//	float theta = i * thetaStep;
	//	for (int j = 0; j < stacks; j++)
	//	{
	//		float phi = j * phiStep;
	//		p->x = radius * cosf(phi) * sinf(theta);
	//		p->y = radius * sinf(phi);
	//		p->z = radius * cosf(phi) * cosf(theta);
	//		p++;
	//		phi += phiStep;
	//		p->x = radius * cosf(phi) * sinf(theta);
	//		p->y = radius * sinf(phi);
	//		p->z = radius * cosf(phi) * cosf(theta);
	//		p++;
	//
	//	}
	//}
	//Create vertex buffer
	{
		D3D11_BUFFER_DESC buffer_desc{};
		buffer_desc.ByteWidth = sizeof(DirectX::XMFLOAT3) * capsuleVertexCount;
		buffer_desc.Usage = D3D11_USAGE_IMMUTABLE;
		buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		buffer_desc.CPUAccessFlags = 0;
		buffer_desc.MiscFlags = 0;
		buffer_desc.StructureByteStride = 0;
		D3D11_SUBRESOURCE_DATA subresource_data{};
		subresource_data.pSysMem = vertices.get();
		subresource_data.SysMemPitch = 0;
		subresource_data.SysMemSlicePitch = 0;
		HRESULT hr = device->CreateBuffer(&buffer_desc, &subresource_data, capsule_vertex_buffer.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), L"Create sphere vertex buffer failed");

	}

}
void DebugRenderer::DrawCylinder(const DirectX::XMFLOAT3& position, float radius, float height, const DirectX::XMFLOAT4& color)
{
	Cylinder cylinder;
	cylinder.position = position;
	cylinder.radius = radius;
	cylinder.height = height;
	cylinder.color = color;
	cylinders.emplace_back(cylinder);
}
void DebugRenderer::DrawSphere(const DirectX::XMFLOAT3& center, float radius, const DirectX::XMFLOAT4& color)
{
	Sphere sphere;
	sphere.center = center;
	sphere.radius = radius;
	sphere.color = color;
	spheres.emplace_back(sphere);
}
void DebugRenderer::DrawCapsule(const DirectX::XMFLOAT3& position, float radius, float height, const DirectX::XMFLOAT4& color)
{
	Capsule capsule;
	capsule.position = position;
	capsule.radius = radius;
	capsule.height = height;
	capsule.color = color;
	capsules.emplace_back(capsule);
}