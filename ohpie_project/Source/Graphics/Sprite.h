#pragma once
#include <d3d11.h>
#include<directxmath.h>
#include<wrl.h>
#include "RenderStates.h"
class Sprite
{
public:
	Sprite();
	Sprite(const wchar_t* filename);
	~Sprite() {};
	struct Vertex
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT4 color;
		DirectX::XMFLOAT2 texcoord;
	};
private:
	
	Microsoft::WRL::ComPtr<ID3D11VertexShader>			vertex_shader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>			pixel_shader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>			input_layout;
	Microsoft::WRL::ComPtr<ID3D11Buffer>				vertex_buffer;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	shader_resource_view;
	D3D11_TEXTURE2D_DESC								texture2d_desc;

	RenderStates::DSS									dss = RenderStates::DSS::ZT_ON_ZW_ON;
	RenderStates::BS									bs = RenderStates::BS::ALPHA;
	RenderStates::SS									ss = RenderStates::SS::LINEAR_SAMPLER_STATE;
public:
	void Render(ID3D11DeviceContext* device_context, float dx, float dy, float dw, float dh);
	void Render(ID3D11DeviceContext* device_context, float dx, float dy, float dw, float dh, float r, float g , float b, float a , float angle );
	void Render(ID3D11DeviceContext* device_context, float dx, float dy, float dw, float dh, float r, float g , float b , float a , float angle , float sx , float sy , float sw , float sh );
	void SetSamplerState(RenderStates::SS ss) { this->ss = ss; }
	void SetDepthStencilState(RenderStates::DSS dss) { this->dss = dss; }
	void SetBlendState(RenderStates::BS bs) { this->bs = bs; }
};