#pragma once
#include <directxmath.h>
class CameraController
{
public:
	CameraController();
	~CameraController() {};
	void Update(float elapsedTime);
private:
	DirectX::XMFLOAT3 camera_eye = { 0.0f,75.0f,-75.0f };
	DirectX::XMFLOAT3 camera_focus = { 0.0f,0.0f,0.0f };
	DirectX::XMFLOAT3 camera_up = { 0.0f,1.0f,0.0f };
	float distance = 0.0f;
	float rotateX = 0.0f;
	float rotateY = 0.0f;
};