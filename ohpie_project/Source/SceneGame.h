#pragma once
#include "Graphics\Sprite.h"
#include <memory>
#include "Graphics\Light.h"
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
private:
	std::unique_ptr<Sprite> sprite[2];
	struct Cuboid
	{
		DirectX::XMFLOAT4 color;
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT3 angle;
		float length;
		float width;
		float height;
	};
	struct Cylinder
	{
		DirectX::XMFLOAT4	color;
		DirectX::XMFLOAT3	position;
		DirectX::XMFLOAT3	angle;
		float				radius;
		float				height;
	};
	struct Sphere
	{
		DirectX::XMFLOAT4	color;
		DirectX::XMFLOAT3	position;
		float				radius;
	};
	
private:
	std::unique_ptr<Light>	directionalLight;
	Cuboid		cuboid{ {1.0f,1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f},5.0f,5.0f,5.0f };
	Cylinder	cylinder{ {1.0f,1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f} ,{0.0f,0.0f,0.0f},5.0f,5.0f };
	Sphere		sphere{ {1.0f,1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},5.0f };
	Sphere		earth{};
	Sphere		moon{};
	DirectX::XMFLOAT4 white{ 1.0f,1.0f,1.0f,1.0f };
	float timer = 0;
	float angle = 0;
	float range = 50;
};