#pragma once
#include <directxmath.h>
class CameraController
{
public:
	CameraController();
	~CameraController() {};
	void Update(float elapsedTime);
private:
	DirectX::XMFLOAT3 camera_eye = { 0,75,-75 };
	DirectX::XMFLOAT3 camera_focus = { 0,0,0 };
	DirectX::XMFLOAT3 camera_up = { 0,1,0 };
	float distance = 0;
	float rotateX = 0;
	float rotateY = 0;
};