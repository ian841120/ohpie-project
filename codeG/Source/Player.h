#pragma once
#include "Graphics\Sprite.h"
#include "Graphics\Graphics.h"
#include "Graphics\Texture.h"
#include <directXmath.h>
#include <memory>
class Player
{
public:
	Player();
	~Player() {};
	void Update();
	void Render();
	Sprite* GetSprite() { return sprite.get(); }
	ShaderId GetshaderType() { return shaderType; }
private:
	std::unique_ptr<Sprite> sprite;
	std::unique_ptr<Texture> maskSprite;6
private:
	DirectX::XMFLOAT2 position;
	int state;
	ShaderId shaderType;
};