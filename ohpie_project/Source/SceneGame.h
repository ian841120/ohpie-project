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
	struct Cuboid
	{
		DirectX::XMFLOAT4 color;
		DirectX::XMFLOAT3 position;
		float length;
		float width;
		float height;
	};

	struct Light
	{
		DirectX::XMFLOAT4 direction;
	};
private:
	Light light{ {0.0f, 0.0f, 0.0f,0.0f} };
	Cuboid cuboid{ {1.0f,1.0f,1.0f,1.0f},{1.0f,1.0f,1.0f},5.0f,5.0f,5.0f };
	float angle = 0.0f;
};