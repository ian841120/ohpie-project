#include "AtmosphericShader.h"
#include "Shader.h"
#include "Graphics.h"
#include "RenderStates.h"
AtmosphericShader::AtmosphericShader()
{
	ID3D11Device* device = Graphics::GetInstance().GetDevice();
	D3D11_INPUT_ELEMENT_DESC inputElementDesc[] =
	{
		{"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0},
	};
	create_vs_from_file(device, "./Shader/AtmosphericShaderVS.cso", vertexShader.GetAddressOf(), inputLayout.GetAddressOf(), inputElementDesc, _countof(inputElementDesc));
	create_ps_from_file(device, "./Shader/AtmosphericShaderPS.cso", pixelShader.GetAddressOf());

	D3D11_BUFFER_DESC desc;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.ByteWidth = sizeof(CbSky);
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;
	HRESULT hr = device->CreateBuffer(&desc, nullptr, skyCbuffer.GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), L"FAIL");
}
void AtmosphericShader::Render(const RenderContext& rc)
{
	rc.deviceContext->VSSetShader(vertexShader.Get(), nullptr, 0);
	rc.deviceContext->PSSetShader(pixelShader.Get(), nullptr, 0);

	rc.deviceContext->IASetInputLayout(inputLayout.Get());
	ID3D11Buffer* constantBuffers[] =
	{
		skyCbuffer.Get(),
	};
	rc.deviceContext->VSSetConstantBuffers(0, ARRAYSIZE(constantBuffers), constantBuffers);
	rc.deviceContext->PSSetConstantBuffers(0, ARRAYSIZE(constantBuffers), constantBuffers);
	const float blend_factor[4] = { 1.0f,1.0f,1.0f,1.0f };
	rc.deviceContext->OMSetBlendState(RenderStates::blendStates[static_cast<int>(RenderStates::BS::ALPHA)].Get(), blend_factor, 0xFFFFFFFF);
	rc.deviceContext->OMSetDepthStencilState(RenderStates::depthStencilStates[static_cast<int>(RenderStates::DSS::ZT_ON_ZW_ON)].Get(), 0);
	rc.deviceContext->RSSetState(RenderStates::rasterizerStates[static_cast<int>(RenderStates::RS::FILL_SOLID)].Get());


}