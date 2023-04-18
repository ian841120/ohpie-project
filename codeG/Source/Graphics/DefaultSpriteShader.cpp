#include "DefaultSpriteShader.h"
#include "Graphics.h"
DefaultSpriteShader::DefaultSpriteShader()
{
	ID3D11Device* device = Graphics::GetInstance().GetDevice();
	//Vertex shader
	{
		D3D11_INPUT_ELEMENT_DESC input_element_desc[]
		{
			{"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"COLOR",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		};

		create_vs_from_file(device, "./Shader/DefaultSpriteShaderVS.cso", vertexShader.GetAddressOf(), inputLayout.GetAddressOf(), input_element_desc, _countof(input_element_desc));
	}
	//Pixel shader
	{
		create_ps_from_file(device, "./Shader/DefaultSpriteShaderPS.cso", pixelShader.GetAddressOf());
	}

}
void DefaultSpriteShader::Begin(const RenderContext&rc)
{

	rc.deviceContext->VSSetShader(vertexShader.Get(), nullptr, 0);
	rc.deviceContext->PSSetShader(pixelShader.Get(), nullptr, 0);
	rc.deviceContext->IASetInputLayout(inputLayout.Get());

	rc.deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	const float blend_factor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	rc.deviceContext->OMSetBlendState(RenderStates::blendStates[static_cast<int>(RenderStates::BS::ALPHA)].Get(), blend_factor, 0xFFFFFFFF);
	rc.deviceContext->PSSetSamplers(0, 1, RenderStates::samplerStates[static_cast<int>(RenderStates::SS::LINEAR_SAMPLER_STATE)].GetAddressOf());
	rc.deviceContext->OMSetDepthStencilState(RenderStates::depthStencilStates[static_cast<int>(RenderStates::DSS::ZT_ON_ZW_ON)].Get(), 1);
	rc.deviceContext->RSSetState(RenderStates::rasterizerStates[static_cast<int>(RenderStates::RS::FILL_SOLID)].Get());


}
void DefaultSpriteShader::Render(const RenderContext& rc, const Sprite* sprite)
{
	UINT stride{ sizeof(Sprite::Vertex) };
	UINT offset{ 0 };
	rc.deviceContext->IASetVertexBuffers(0, 1, sprite->GetVertexBuffer().GetAddressOf(), &stride, &offset);
	rc.deviceContext->PSSetShaderResources(0, 1, sprite->GetShaderResourceView().GetAddressOf());
	rc.deviceContext->Draw(4, 0);
}
void DefaultSpriteShader::End(const RenderContext& rc)
{
	rc.deviceContext->VSSetShader(nullptr, nullptr, 0);
	rc.deviceContext->PSSetShader(nullptr, nullptr, 0);
	rc.deviceContext->IASetInputLayout(nullptr);

	ID3D11ShaderResourceView* srv = { nullptr };
	rc.deviceContext->PSSetShaderResources(0, 1, &srv);

}