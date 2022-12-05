#include "CameraController.h"
#include "Camera.h"
#include "Input\InputClass.h"
#define MOVESPEED 0.2f
#define ROTATESPEED 0.1f
CameraController::CameraController()
{
	Camera& camera = Camera::Instance();
	camera.SetLookAt(
		camera_eye,
		camera_focus,
		camera_up
	);
	camera.SetPerspectiveFov(
		DirectX::XMConvertToRadians(25.0f),
		1280.0f / 720.0f,
		1.0f,
		10000.0f
	);
	DirectX::XMVECTOR Eye = DirectX::XMLoadFloat3(&camera.GetEye());
	DirectX::XMVECTOR Focus = DirectX::XMLoadFloat3(&camera.GetFocus());
	DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(Eye, Focus);
	DirectX::XMVECTOR Length = DirectX::XMVector3Length(Vec);
	DirectX::XMStoreFloat(&distance, Length);
	DirectX::XMFLOAT3 vec;
	DirectX::XMStoreFloat3(&vec, DirectX::XMVectorDivide(Vec, Length));

	float f = sqrtf(vec.x * vec.x + vec.z * vec.z);
	// X axis 
	rotateX = atan2f(vec.y, f);
	//Y axis
	rotateY = atan2f(vec.x, vec.z);
}
void CameraController::Update(float elapsedTime)
{
	MouseClass& mouse = InputClass::Instance().GetMouse();

	float moveX = (mouse.GetCursorPositionX() - mouse.GetPreCursorPositionX()) * MOVESPEED;
	float moveY = (mouse.GetCursorPositionY() - mouse.GetPreCursorPositionY()) * MOVESPEED;
	DirectX::XMMATRIX View;
	Camera& camera = Camera::Instance();
	if (mouse.GetButtonState(MouseClass::MOUSEKEY::RBUTTON))
	{
		rotateY += moveX * ROTATESPEED;
		if (rotateY > DirectX::XM_PI)
			rotateY -= DirectX::XM_2PI;
		else if (rotateY < -DirectX::XM_PI)
			rotateY += DirectX::XM_2PI;
		rotateX += moveY * ROTATESPEED;
		if (rotateX > DirectX::XMConvertToRadians(89.9f))
			rotateX = DirectX::XMConvertToRadians(89.9f);
		else if (rotateX < -DirectX::XMConvertToRadians(89.9f))
			rotateX = -DirectX::XMConvertToRadians(89.9f);
	}
	else if (mouse.GetButtonState(MouseClass::MOUSEKEY::MBUTTON))
	{
		View = DirectX::XMMatrixLookAtLH(DirectX::XMLoadFloat3(&camera_eye),
										DirectX::XMLoadFloat3(&camera_focus),
										DirectX::XMLoadFloat3(&camera_up));
		DirectX::XMFLOAT4X4 world;
		DirectX::XMStoreFloat4x4(&world, DirectX::XMMatrixInverse(nullptr, View));
		float step = distance * 0.005f;
		float x = moveX * step;
		float y = moveY * step;
		camera_focus.x -= world._11 * x;
		camera_focus.y -= world._12 * x;
		camera_focus.z -= world._13 * x;

		camera_focus.x += world._21 * y;
		camera_focus.y += world._22 * y;
		camera_focus.z += world._23 * y;
	}
	float sx = sinf(rotateX);
	float cx = cosf(rotateX);
	float sy = sinf(rotateY);
	float cy = cosf(rotateY);
	DirectX::XMVECTOR Focus = DirectX::XMLoadFloat3(&camera_focus);
	DirectX::XMMATRIX Transform = DirectX::XMMatrixRotationRollPitchYaw(rotateX, rotateY, 0);
	DirectX::XMVECTOR Front = Transform.r[2];
	DirectX::XMVECTOR Distance = DirectX::XMVectorSet(distance, distance, distance, 0.0f);
	Front = DirectX::XMVectorMultiply(Front, Distance);
	DirectX::XMVECTOR Eye = DirectX::XMVectorSubtract(Focus, Front);
	DirectX::XMStoreFloat3(&camera_eye, Eye);
	camera.SetLookAt(camera_eye, camera_focus, camera_up);

	// Zoom
	if (mouse.GetWheel() != 0)
	{
		distance -= static_cast<float>(mouse.GetWheel()) * distance * 0.001f;
	}

}