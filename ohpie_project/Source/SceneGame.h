#pragma once
#include "Graphics\Sprite.h"
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
	std::unique_ptr<Sprite> sprite[2];
	float x = 0;
	float y = 0;
};