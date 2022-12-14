#include "Sprite.h"
#include "Graphics.h"
#include <memory>
#include <WICTextureLoader.h>
Sprite::Sprite() :Sprite(nullptr)
{

}
Sprite::Sprite(const wchar_t* filename)
{
	ID3D11Device* device = Graphics::GetInstance().GetDevice();
	HRESULT hr = { S_OK };
	//Set Vertices data 
	Vertex vertices[]
	{
		{{-0.5,+0.5,0},{1,0,0,1},{0,0}},
		{{+0.5,+0.5,0},{0,1,0,1},{1,0}},
		{{-0.5,-0.5,0},{0,0,1,1},{0,1}},
		{{+0.5,-0.5,0},{0,0,0,1},{1,1}},
	};
	//Create Vertex Buffer
	{
		D3D11_BUFFER_DESC buffer_desc{};
		buffer_desc.ByteWidth = sizeof(vertices);
		buffer_desc.Usage = D3D11_USAGE_DYNAMIC;
		buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		buffer_desc.MiscFlags = 0;
		buffer_desc.StructureByteStride = 0;
		D3D11_SUBRESOURCE_DATA subresource_data{};
		subresource_data.pSysMem = vertices;
		subresource_data.SysMemPitch = 0;
		subresource_data.SysMemSlicePitch = 0;
		hr = device->CreateBuffer(&buffer_desc, &subresource_data, vertex_buffer.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), L"Create vertex buffer failed");

	}

	//Vertex shader
	{
		// Read cso file
		FILE* fp{ nullptr };
		fopen_s(&fp, "./Shader/SpriteVS.cso", "rb");
		_ASSERT_EXPR(fp, L"VS CSO File not found");
		fseek(fp, 0, SEEK_END);
		long cso_sz{ ftell(fp) };
		fseek(fp, 0, SEEK_SET);
		std::unique_ptr<unsigned char[]>cso_data{ std::make_unique<unsigned char[]>(cso_sz) };
		fread(cso_data.get(), cso_sz, 1, fp);
		fclose(fp);
		//Create vertex shader
		hr = device->CreateVertexShader(cso_data.get(), cso_sz, nullptr, vertex_shader.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), L"Failed to create vertexshader");
		//Create input layout
		D3D11_INPUT_ELEMENT_DESC input_element_desc[]
		{
			{"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"COLOR",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		};
		hr = device->CreateInputLayout(input_element_desc, _countof(input_element_desc), cso_data.get(), cso_sz, input_layout.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), L"Failed to create inputlayout");
	}
	//Pixel shader
	{
		//Read cso file
		FILE* fp{ nullptr };
		fopen_s(&fp, "./Shader/SpritePS.cso", "rb");
		_ASSERT_EXPR(fp, L"PS CSO File not found");
		fseek(fp, 0, SEEK_END);
		long cso_sz{ ftell(fp) };
		fseek(fp, 0, SEEK_SET);
		std::unique_ptr<unsigned char[]>cso_data{ std::make_unique<unsigned char[]>(cso_sz) };
		fread(cso_data.get(), cso_sz, 1, fp);
		fclose(fp);
		//Create Pixel shader
		hr = device->CreatePixelShader(cso_data.get(), cso_sz, nullptr, pixel_shader.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), L"Failed to create pixelshader");
	}
	if (filename != nullptr)
	{
		//Read texture from file
		Microsoft::WRL::ComPtr<ID3D11Resource> resource;
		hr = DirectX::CreateWICTextureFromFile(device, filename, resource.GetAddressOf(), shader_resource_view.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), L"Failed to create texture");
		//Get texture description

		Microsoft::WRL::ComPtr<ID3D11Texture2D>texture2d;
		hr = resource->QueryInterface<ID3D11Texture2D>(texture2d.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), L"Get texture2d address failed");
		texture2d->GetDesc(&texture2d_desc);


	}
	else
	{
		//Create dummy texture
		const int width = 8;
		const int height = 8;
		UINT pixels[width * height];
		::memset(pixels, 0xFF, sizeof(pixels));

		D3D11_TEXTURE2D_DESC texture_desc = {};
		texture_desc.Width = width;
		texture_desc.Height = height;
		texture_desc.MipLevels = 1;
		texture_desc.ArraySize = 1;
		texture_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		texture_desc.SampleDesc.Count = 1;
		texture_desc.SampleDesc.Quality = 0;
		texture_desc.Usage = D3D11_USAGE_DEFAULT;
		texture_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		texture_desc.CPUAccessFlags = 0;
		texture_desc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA data;
		::memset(&data, 0, sizeof(data));
		data.pSysMem = pixels;
		data.SysMemPitch = width;

		Microsoft::WRL::ComPtr<ID3D11Texture2D>	texture2d;
		HRESULT hr = device->CreateTexture2D(&texture_desc, &data, texture2d.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), L"Create dummy texture failed");

		hr = device->CreateShaderResourceView(texture2d.Get(), nullptr, shader_resource_view.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), L"Create dummy shader resource view failed");

		texture2d->GetDesc(&texture2d_desc);
	}

}
void Sprite::Render(ID3D11DeviceContext* device_context, float dx, float dy, float dw, float dh)
{
	Render(device_context, dx, dy, dw, dh, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, static_cast<float>(texture2d_desc.Width), static_cast<float>(texture2d_desc.Height));
}
void Sprite::Render(ID3D11DeviceContext* device_context, float dx, float dy, float dw, float dh, float r, float g, float b, float a, float angle)
{
	Render(device_context, dx, dy, dw, dh, r, g, b, a, angle, 0.0f, 0.0f, static_cast<float>(texture2d_desc.Width), static_cast<float>(texture2d_desc.Height));

}
void Sprite::Render(ID3D11DeviceContext* device_context,float dx,float dy,float dw,float dh,float r,float g,float b,float a,float angle,float sx,float sy,float sw,float sh)
{
	D3D11_VIEWPORT viewport{};
	UINT num_viewport{ 1 };
	device_context->RSGetViewports(&num_viewport, &viewport);
	//Set sprite`s corrdinate
	//  (x0,y0)*--------*(x1,y1)
	//		   |		|
	//		   |		|
	//		   |		|
	//		   |		|
	//		   |		|
	//		   |		|
	//  (x2,y2)*--------*(x3,y3)
	DirectX::XMFLOAT2 positions[] =
	{
		DirectX::XMFLOAT2(dx,dy),
		DirectX::XMFLOAT2(dx + dw,dy),
		DirectX::XMFLOAT2(dx,dy + dh),
		DirectX::XMFLOAT2(dx + dw,dy + dh),
	};
	DirectX::XMFLOAT2 texcoord[] =
	{
		DirectX::XMFLOAT2(sx,sy),
		DirectX::XMFLOAT2(sx+sw,sy),
		DirectX::XMFLOAT2(sx,sy+sh),
		DirectX::XMFLOAT2(sx+sw,sy+sh),
	};
	//Rotation
	//Translate sprite`s center to origin
	float cx = dx + dw * 0.5f;
	float cy = dy + dh * 0.5f;
	for (auto& p : positions)
	{
		p.x -= cx;
		p.y -= cy;
	}
	//Rotate vertices by angle
	float tx, ty;
	float cos = cosf(DirectX::XMConvertToRadians(angle));
	float sin = sinf(DirectX::XMConvertToRadians(angle));
	for (auto& p : positions)
	{
		tx = p.x;
		ty = p.y;
		p.x = cos * tx - sin * ty;
		p.y = sin * tx + cos * ty;
	}
	//Translate center to origin position
	for (auto& p : positions)
	{
		p.x += cx;
		p.y += cy;
	}
	

	//Convet to NDC space
	for (auto& p : positions)
	{
		p.x = 2.0f * p.x / viewport.Width - 1.0f;
		p.y = 1.0f - 2.0f * p.y / viewport.Height;
	}
	
	HRESULT hr{ S_OK };
	//Edit vertex data 
	D3D11_MAPPED_SUBRESOURCE mapped_subresource{};
	hr = device_context->Map(vertex_buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_subresource);
	_ASSERT_EXPR(SUCCEEDED(hr), L"Failed to Mapping");
	//Used pData to rewrite vertex data
	Vertex* vertices{ reinterpret_cast<Vertex*>(mapped_subresource.pData) };
	if (vertices != nullptr)
	{
		for (int i = 0; i < 4; i++)
		{
			vertices[i].position.x = positions[i].x;
			vertices[i].position.y = positions[i].y;
			vertices[i].position.z = 0;
			vertices[i].color = { r,g,b,a };
			vertices[i].texcoord.x = texcoord[i].x / texture2d_desc.Width;
			vertices[i].texcoord.y = texcoord[i].y / texture2d_desc.Height;
		}
	}
	device_context->Unmap(vertex_buffer.Get(), 0);

	UINT stride{ sizeof(Vertex) };
	UINT offset{ 0 };
	device_context->IASetVertexBuffers(0, 1, vertex_buffer.GetAddressOf(), &stride, &offset);
	device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	device_context->IASetInputLayout(input_layout.Get());
	device_context->PSSetShaderResources(0, 1, shader_resource_view.GetAddressOf());

	device_context->PSSetSamplers(0, 1, RenderStates::samplerStates[static_cast<int>(ss)].GetAddressOf());
	device_context->OMSetDepthStencilState(RenderStates::depthStencilStates[static_cast<int>(dss)].Get(), 1);
	device_context->OMSetBlendState(RenderStates::blendStates[static_cast<int>(bs)].Get(), nullptr, 0xFFFFFFFF);

	device_context->RSSetState(RenderStates::rasterizerStates[static_cast<int>(RenderStates::RS::FILL_SOLID)].Get());
	device_context->VSSetShader(vertex_shader.Get(), nullptr, 0);
	device_context->PSSetShader(pixel_shader.Get(), nullptr, 0);
	device_context->Draw(4, 0);
	

}