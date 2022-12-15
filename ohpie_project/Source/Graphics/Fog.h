#pragma once
#include <directxmath.h>
#include "RenderContext.h"
class Fog
{
public:
	Fog() {};
	~Fog() {};
	void pushRenderContext(RenderContext& rc);
	void DrawDebugGui();
	void setFogColor(DirectX::XMFLOAT4 c) { fogColor = c; }
	const DirectX::XMFLOAT4& getFogColor() { return fogColor; }

	void setFogStart(float fs) { fogStart = fs; }
	const float GetFogStart(){ return fogStart; }

	void setFogRange(float fr) { fogRange = fr; }
	const float GetFogRange() { return fogRange; }
private:
	DirectX::XMFLOAT4 fogColor = { 0.5f,0.5f,0.5f,1.0f };
	float fogStart = 90.0f;
	float fogRange = 60.0f;

};