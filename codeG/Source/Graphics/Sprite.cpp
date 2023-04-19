#include "Sprite.h"
#include "Shader.h"
#include "Graphics.h"
#include <memory>
#include <WICTextureLoader.h>
Sprite::Sprite()
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

		textureWidth = texture_desc.Width;
		textureHeight = texture_desc.Height;
	}

}
void Sprite::Update(float dx, float dy, float dw, float dh)
{
	Update(dx, dy, dw, dh, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, static_cast<float>(texture2d_desc.Width), static_cast<float>(texture2d_desc.Height));
}
void Sprite::Update(float dx, float dy, float dw, float dh, float r, float g, float b, float a, float angle)
{
	Update(dx, dy, dw, dh, r, g, b, a, angle, 0.0f, 0.0f, static_cast<float>(texture2d_desc.Width), static_cast<float>(texture2d_desc.Height));

}
void Sprite::Update(float dx,float dy,float dw,float dh,float r,float g,float b,float a,float angle,float sx,float sy,float sw,float sh)
{
	ID3D11DeviceContext* device_context = Graphics::GetInstance().GetDeviceContext();
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
	dx = calcPovit(dx, dy, dw, dh).x;
	dy = calcPovit(dx, dy, dw, dh).y;
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
}
void Sprite::SetShaderResourceView(const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& srv, int texWidth, int texHeight)
{
	shader_resource_view = srv;
	textureWidth = texWidth;
	textureHeight = texHeight;

}
DirectX::XMFLOAT2 Sprite::calcPovit(float dx,float dy,float dw,float dh)
{
	switch (povit)
	{
	case POVIT::CENTER:
		dx = dx - dw / 2;
		dy = dy - dh / 2;
		break;
	case POVIT::UPPER_LEFT:
		dx = dx; 
		dy = dy;
		break;
	case POVIT::UPPER_RIGHT:
		dx = dx - dw;
		dy = dy ;
		break;
	case POVIT::LOWER_LEFT:
		dx = dx ;
		dy = dy - dh;
		break;
	case POVIT::LOWER_RIGHT:
		dx = dx - dw;
		dy = dy - dh;
		break;
	case POVIT::CENTER_LEFT:
		dx = dx;
		dy = dy - dh / 2;
		break;
	case POVIT::CENTER_RIGHT:
		dx = dx - dw;
		dy = dy - dh / 2;;
		break;
	case POVIT::CENTER_UPPER:
		dx = dx - dw / 2;
		dy = dy;
		break;
	case POVIT::CENTER_LOWER:
		dx = dx - dw/2;
		dy = dy - dh;
		break;
	}
	return DirectX::XMFLOAT2(dx, dy);
}