#pragma once
#include <d3d11.h>
#include <wrl.h>
#include "RenderContext.h"
#include "Shader.h"
#include "Sprite.h"
class DefaultSpriteShader:public SpriteShader
{
public:
	DefaultSpriteShader();
	~DefaultSpriteShader() {};
	void Begin(const RenderContext&rc) override;
	void Render(const RenderContext& rc, const Sprite* sprite)override;
	void End(const RenderContext& rc)override;
private:
	Microsoft::WRL::ComPtr<ID3D11VertexShader>	vertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>	pixelShader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>   inputLayout;

};