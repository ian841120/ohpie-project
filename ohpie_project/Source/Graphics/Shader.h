#pragma once
#include <d3d11.h>
#include <memory>
#include "RenderContext.h"
#include "Model.h"
#include "Sprite.h"
void create_vs_from_file(ID3D11Device* device, const char* filename, ID3D11VertexShader** vertexShader, ID3D11InputLayout** inputLayout, D3D11_INPUT_ELEMENT_DESC* inputDesc, int inputCount);
void create_ps_from_file(ID3D11Device* device, const char* filename, ID3D11PixelShader** pixelShader);
class Shader
{
public:
	Shader() {};
	virtual ~Shader() {};
	// draw begin
	virtual void Begin(ID3D11DeviceContext* dc, const RenderContext& rc) = 0;
	//draw
	virtual void Render(ID3D11DeviceContext* dc, const Model* model) = 0;
	//draw final
	virtual void End(ID3D11DeviceContext* dc) = 0;
};

class SpriteShader
{
public:
	SpriteShader() {};
	virtual ~SpriteShader() {};
	// draw begin
	virtual void Begin(const RenderContext& rc) = 0;
	//draw
	virtual void Render(const RenderContext& rc, const Sprite* sprite) = 0;
	//draw final
	virtual void End(const RenderContext& rc) = 0;
};