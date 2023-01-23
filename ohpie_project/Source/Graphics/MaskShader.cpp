#include "MaskShader.h"
#include "Graphics.h"
#include "RenderStates.h"
MaskShader::MaskShader()
{
	ID3D11Device* device = Graphics::GetInstance().GetDevice();
	D3D11_INPUT_ELEMENT_DESC inputElementDesc[] =
	{
		{"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,		0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR",	0,DXGI_FORMAT_R32G32B32A32_FLOAT,	0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,			0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	create_vs_from_file(device, "./Shader/MaskShaderVS.cso", vertexShader.GetAddressOf(), inputLayout.GetAddressOf(), inputElementDesc, _countof(inputElementDesc));
	create_ps_from_file(device, "./Shader/MaskShaderPS.cso", pixelShader.GetAddressOf());
	
	D3D11_BUFFER_DESC bufferDesc;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(CbMask);
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	HRESULT hr = device->CreateBuffer(&bufferDesc, 0, maskConstantBuffer.GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), L"FAILED");
}
void MaskShader::Begin(const RenderContext& rc)
{
	ID3D11Buffer* constBuffers[] =
	{
		maskConstantBuffer.Get(),
	};
	rc.deviceContext->VSSetConstantBuffers(0, ARRAYSIZE(constBuffers), constBuffers);
	rc.deviceContext->PSSetConstantBuffers(0, ARRAYSIZE(constBuffers), constBuffers);

	rc.deviceContext->VSSetShader(vertexShader.Get(), nullptr, 0);
	rc.deviceContext->PSSetShader(pixelShader.Get(), nullptr, 0);
	rc.deviceContext->IASetInputLayout(inputLayout.Get());

	rc.deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	const float blend_factor[4] = { 1.0f,1.0f,1.0f,1.0f };
	rc.deviceContext->OMSetBlendState(RenderStates::blendStates[static_cast<int>(RenderStates::BS::ALPHA)].Get(), blend_factor, 0xFFFFFFFF);
	rc.deviceContext->OMSetDepthStencilState(RenderStates::depthStencilStates[static_cast<int>(RenderStates::DSS::ZT_ON_ZW_ON)].Get(), 0);
	rc.deviceContext->RSSetState(RenderStates::rasterizerStates[static_cast<int>(RenderStates::RS::FILL_SOLID)].Get());
	rc.deviceContext->PSSetSamplers(0, 1, RenderStates::samplerStates[static_cast<int>(RenderStates::SS::LINEAR_SAMPLER_STATE)].GetAddressOf());
}
void MaskShader::Render(const RenderContext& rc, const Sprite* sprite)
{
	CbMask	cbMask;
	cbMask.dissolveThreshold = rc.maskData.dissolveThreshold;
	cbMask.edgeThreshold = rc.maskData.edgeThreshold;
	cbMask.edgeColor = rc.maskData.edgeColor;
	rc.deviceContext->UpdateSubresource(maskConstantBuffer.Get(), 0, 0, &cbMask, 0, 0);

	UINT stride = sizeof(Sprite::Vertex);
	UINT offset = 0;
	rc.deviceContext->IASetVertexBuffers(0, 1, sprite->GetVertexBuffer().GetAddressOf(), &stride, &offset);
	ID3D11ShaderResourceView* srvs[] =
	{
		sprite->GetShaderResourceView().Get(),
		rc.maskData.maskTexture,
	};
	rc.deviceContext->PSSetShaderResources(0, ARRAYSIZE(srvs), srvs);
	rc.deviceContext->Draw(4, 0);
}
void MaskShader::End(const RenderContext& rc)
{
	rc.deviceContext->VSSetShader(nullptr, nullptr, 0);
	rc.deviceContext->PSSetShader(nullptr, nullptr, 0);
	rc.deviceContext->IASetInputLayout(nullptr);
	ID3D11ShaderResourceView* srvs[] = { nullptr,nullptr };
	rc.deviceContext->PSSetShaderResources(0, ARRAYSIZE(srvs), srvs);
}

