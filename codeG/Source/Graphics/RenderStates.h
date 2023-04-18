#pragma once
#include <d3d11.h>
#include <wrl.h>
class RenderStates
{
public:
	RenderStates(ID3D11Device* device);
	~RenderStates() {};
public:
	enum class BS //blend state
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
	enum class SS	//sampler state
	{
		POINT_SAMPLER_STATE,
		LINEAR_SAMPLER_STATE,
		ANISOTROPIC_SAMPLER_STATE,
		SAMPLER_STATE_MAX
	};
	enum class DSS  //depth_stencil_state
	{
		ZT_ON_ZW_ON,
		ZT_ON_ZW_OFF,
		ZT_OFF_ZW_ON,
		ZT_OFF_ZW_OFF,
		DEPTH_STENCIL_STATE_MAX
	};
	enum class RS  //rasterizerState
	{
		FILL_SOLID,
		FILL_WIREFRAME,
		RASTERIZER_STATE_MAX
	};
public:
	static Microsoft::WRL::ComPtr<ID3D11BlendState>				blendStates[static_cast<int>(BS::BLENDER_STATE_MAX)];
	static Microsoft::WRL::ComPtr<ID3D11SamplerState>			samplerStates[static_cast<int>(SS::SAMPLER_STATE_MAX)];
	static Microsoft::WRL::ComPtr<ID3D11DepthStencilState>		depthStencilStates[static_cast<int>(DSS::DEPTH_STENCIL_STATE_MAX)];
	static Microsoft::WRL::ComPtr<ID3D11RasterizerState>		rasterizerStates[static_cast<int>(RS::RASTERIZER_STATE_MAX)];

};