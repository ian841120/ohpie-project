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
	void Update(float elapsedTime);
	void Render(RenderContext&rc);
	void DebugGUI();
	Sprite* GetSprite() { return sprite.get(); }
	ShaderId GetshaderType() { return shaderType; }
private:
	std::unique_ptr<Sprite> sprite;
	std::unique_ptr<Texture> playerTexture;
	std::unique_ptr<Texture> maskTexture;
private:
	DirectX::XMFLOAT2 position;
	float width;
	float height;
	int state;
	int disappeareTime;
	int speed;
	int flip;
	Sprite::POVIT povit = Sprite::POVIT::CENTER;
	ShaderId shaderType;
	MaskData maskData;
};