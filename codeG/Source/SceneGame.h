#pragma once
#include "Graphics\Sprite.h"
#include "Graphics\Texture.h"
#include "Player.h"
#include <memory>

class SceneGame
{
public:
	SceneGame() {};
	~SceneGame() {};
	void Initialize();
	void Finalize();
	void Update(float elapsed_time);
	void Render();
	
private:
	void DrawGrid();
	void DrawDebugGUI();
	void DrawTextureDebugGUI();
private:
private:
	std::unique_ptr<Texture> maskTexture;
	std::unique_ptr<Texture> texture[2];
	std::unique_ptr<Player>player;
};