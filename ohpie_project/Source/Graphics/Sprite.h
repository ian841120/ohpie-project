#pragma once
#include <d3d11.h>
#include<directxmath.h>
#include<wrl.h>
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
	enum class SAMPLER_STATE
	{
		POINT_SAMPLER_STATE,
		LINEAR_SAMPLER_STATE,
		ANISOTROPIC_SAMPLER_STATE,
		SAMPLER_STATE_MAX
	};
	enum class DEPTH_STENCIL_STATE
	{
		ZT_ON_ZW_ON,
		ZT_ON_ZW_OFF,
		ZT_OFF_ZW_ON,
		ZT_OFF_ZW_OFF,
		DEPTH_STENCIL_STATE_MAX
	};
	enum class BLENDER_STATE
	{
		NONE, 
		ALPHA, 
		ADD, 
		SUBTRACT, 
		REPLACE,
		MULTIPLY, 
		LIGHTEN, 
		DARKEN, 
		SCREEN, 
		BLENDER_STATE_MAX
	};
private:
	
	Microsoft::WRL::ComPtr<ID3D11VertexShader>			vertex_shader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>			pixel_shader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>			input_layout;
	Microsoft::WRL::ComPtr<ID3D11Buffer>				vertex_buffer;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	shader_resource_view;
	D3D11_TEXTURE2D_DESC								texture2d_desc;
	Microsoft::WRL::ComPtr<ID3D11SamplerState>			sampler_states[static_cast<int>(SAMPLER_STATE::SAMPLER_STATE_MAX)];
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState>		depth_stencil_states[static_cast<int>(DEPTH_STENCIL_STATE::DEPTH_STENCIL_STATE_MAX)];
	Microsoft::WRL::ComPtr<ID3D11BlendState>			blender_states[static_cast<int>(BLENDER_STATE::BLENDER_STATE_MAX)];
	Microsoft::WRL::ComPtr<ID3D11RasterizerState>		rasterizer_state;
	SAMPLER_STATE										ss = SAMPLER_STATE::POINT_SAMPLER_STATE;
	DEPTH_STENCIL_STATE									dss = DEPTH_STENCIL_STATE::ZT_ON_ZW_ON;
	BLENDER_STATE										bs = BLENDER_STATE::NONE;
public:
	void render(ID3D11DeviceContext* device_context, float dx, float dy, float dw, float dh);
	void render(ID3D11DeviceContext* device_context, float dx, float dy, float dw, float dh, float r, float g , float b, float a , float angle );
	void render(ID3D11DeviceContext* device_context, float dx, float dy, float dw, float dh, float r, float g , float b , float a , float angle , float sx , float sy , float sw , float sh );
	void SetSamplerState(SAMPLER_STATE ss) { this->ss = ss; }
	void SetDepthStencilState(DEPTH_STENCIL_STATE dss) { this->dss = dss; }
	void SetBlenderState(BLENDER_STATE bs) { this->bs = bs; }
};