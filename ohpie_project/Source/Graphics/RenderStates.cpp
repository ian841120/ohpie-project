#include "RenderStates.h"
Microsoft::WRL::ComPtr<ID3D11BlendState>			RenderStates::blendStates[static_cast<int>(BS::BLENDER_STATE_MAX)] = {};
Microsoft::WRL::ComPtr<ID3D11SamplerState>			RenderStates::samplerStates[static_cast<int>(SS::SAMPLER_STATE_MAX)] = {};
Microsoft::WRL::ComPtr<ID3D11DepthStencilState>		RenderStates::depthStencilStates[static_cast<int>(DSS::DEPTH_STENCIL_STATE_MAX)] = {};
Microsoft::WRL::ComPtr<ID3D11RasterizerState>		RenderStates::rasterizerStates[static_cast<int>(RS::RASTERIZER_STATE_MAX)] = {};

RenderStates::RenderStates(ID3D11Device* device)
{
	HRESULT hr{ S_OK };

	//Create blender states
	{
		D3D11_BLEND_DESC blendDesc = {};
		//  NONE
		blendDesc.AlphaToCoverageEnable = FALSE;
		blendDesc.IndependentBlendEnable = FALSE;
		blendDesc.RenderTarget[0].BlendEnable = FALSE;
		blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
		blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		hr = device->CreateBlendState(&blendDesc, blendStates[static_cast<int>(BS::NONE)].GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), L"Create blender state failed");
		//  ALPHA
		blendDesc.AlphaToCoverageEnable = FALSE;
		blendDesc.IndependentBlendEnable = FALSE;
		blendDesc.RenderTarget[0].BlendEnable = TRUE;
		blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
		blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		hr = device->CreateBlendState(&blendDesc, blendStates[static_cast<int>(BS::ALPHA)].GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), L"Create blender state failed");
		//  ADD
		blendDesc.AlphaToCoverageEnable = FALSE;
		blendDesc.IndependentBlendEnable = FALSE;
		blendDesc.RenderTarget[0].BlendEnable = TRUE;
		blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
		blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		hr = device->CreateBlendState(&blendDesc, blendStates[static_cast<int>(BS::ADD)].GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), L"Create blender state failed");
		//  SUBTRACT
		blendDesc.AlphaToCoverageEnable = FALSE;
		blendDesc.IndependentBlendEnable = FALSE;
		blendDesc.RenderTarget[0].BlendEnable = TRUE;
		blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ZERO;
		blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_COLOR;
		blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
		blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		hr = device->CreateBlendState(&blendDesc, blendStates[static_cast<int>(BS::SUBTRACT)].GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), L"Create blender state failed");
		//	REPLACE
		blendDesc.AlphaToCoverageEnable = FALSE;
		blendDesc.IndependentBlendEnable = FALSE;
		blendDesc.RenderTarget[0].BlendEnable = TRUE;
		blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
		blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		hr = device->CreateBlendState(&blendDesc, blendStates[static_cast<int>(BS::REPLACE)].GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), L"Create blender state failed");
		//	MULTIPLY
		blendDesc.AlphaToCoverageEnable = FALSE;
		blendDesc.IndependentBlendEnable = FALSE;
		blendDesc.RenderTarget[0].BlendEnable = TRUE;
		blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ZERO;
		blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_SRC_COLOR;
		blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_DEST_ALPHA;
		blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		hr = device->CreateBlendState(&blendDesc, blendStates[static_cast<int>(BS::MULTIPLY)].GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), L"Create blender state failed");
		//	LIGHTEN
		blendDesc.AlphaToCoverageEnable = FALSE;
		blendDesc.IndependentBlendEnable = FALSE;
		blendDesc.RenderTarget[0].BlendEnable = TRUE;
		blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_MAX;
		blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_MAX;
		blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		hr = device->CreateBlendState(&blendDesc, blendStates[static_cast<int>(BS::LIGHTEN)].GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), L"Create blender state failed");
		//	DARKEN
		blendDesc.AlphaToCoverageEnable = FALSE;
		blendDesc.IndependentBlendEnable = FALSE;
		blendDesc.RenderTarget[0].BlendEnable = TRUE;
		blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_MIN;
		blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_MIN;
		blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		hr = device->CreateBlendState(&blendDesc, blendStates[static_cast<int>(BS::DARKEN)].GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), L"Create blender state failed");
		//	SCREEN
		blendDesc.AlphaToCoverageEnable = FALSE;
		blendDesc.IndependentBlendEnable = FALSE;
		blendDesc.RenderTarget[0].BlendEnable = TRUE;
		blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_COLOR;
		blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
		blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		hr = device->CreateBlendState(&blendDesc, blendStates[static_cast<int>(BS::SCREEN)].GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), L"Create blender state failed");
	}

	//Create Sampler states
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
	// POINT
	sampler_desc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	hr = device->CreateSamplerState(&sampler_desc, samplerStates[static_cast<int>(SS::POINT_SAMPLER_STATE)].GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), L"Create sampler state failed");
	//LINEAR
	sampler_desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	hr = device->CreateSamplerState(&sampler_desc, samplerStates[static_cast<int>(SS::LINEAR_SAMPLER_STATE)].GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), L"Create sampler state failed");
	//ANISOTROPIC
	sampler_desc.Filter = D3D11_FILTER_ANISOTROPIC;
	hr = device->CreateSamplerState(&sampler_desc, samplerStates[static_cast<int>(SS::ANISOTROPIC_SAMPLER_STATE)].GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), L"Create sampler state failed");
	}
	// Create Depth states
	{
		D3D11_DEPTH_STENCIL_DESC depth_stencil_desc{};
		depth_stencil_desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
		//ZT_ON_ZW_ON
		depth_stencil_desc.DepthEnable = TRUE;
		depth_stencil_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		hr = device->CreateDepthStencilState(&depth_stencil_desc, depthStencilStates[static_cast<int>(DSS::ZT_ON_ZW_ON)].GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), L"Create depth stencil failed");
		//ZT_ON_ZW_OFF
		depth_stencil_desc.DepthEnable = TRUE;
		depth_stencil_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		hr = device->CreateDepthStencilState(&depth_stencil_desc, depthStencilStates[static_cast<int>(DSS::ZT_ON_ZW_OFF)].GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), L"Create depth stencil failed");
		//ZT_OFF_ZW_ON
		depth_stencil_desc.DepthEnable = FALSE;
		depth_stencil_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		hr = device->CreateDepthStencilState(&depth_stencil_desc, depthStencilStates[static_cast<int>(DSS::ZT_OFF_ZW_ON)].GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), L"Create depth stencil failed");
		//ZT_OFF_ZW_OFF
		depth_stencil_desc.DepthEnable = FALSE;
		depth_stencil_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		hr = device->CreateDepthStencilState(&depth_stencil_desc, depthStencilStates[static_cast<int>(DSS::ZT_OFF_ZW_OFF)].GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), L"Create depth stencil failed");
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
		hr = device->CreateRasterizerState(&rasterize_desc, rasterizerStates[static_cast<int>(RS::FILL_SOLID)].GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), L"Failed to create rasterizer state");
		rasterize_desc.FillMode = D3D11_FILL_WIREFRAME;
		hr = device->CreateRasterizerState(&rasterize_desc, rasterizerStates[static_cast<int>(RS::FILL_WIREFRAME)].GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), L"Failed to create rasterizer state");

	}

}