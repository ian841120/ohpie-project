#pragma once
#include <d3d11.h>
#include <wrl.h>
#include "shader.h"
#include "RenderContext.h"
#include "Sprite.h"
class MaskShader:public SpriteShader
{
public:
	MaskShader();
	~MaskShader() override{};
	void Begin(const RenderContext& rc)override;
	void Render(const RenderContext& rc,const Sprite* sprite)override;
	void End(const RenderContext& rc)override;
private:
	struct CbMask
	{
		float dissolveThreshold;
		float edgeThreshold;
		DirectX::XMFLOAT2 path;
		DirectX::XMFLOAT4 edgeColor;
	};
private:
	Microsoft::WRL::ComPtr<ID3D11VertexShader>	vertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>	pixelShader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>	inputLayout;

	Microsoft::WRL::ComPtr<ID3D11Buffer>		maskConstantBuffer;
};