#include "Camera.h"
void Camera::setLookAt(const DirectX::XMFLOAT3& eye,const DirectX::XMFLOAT3& focus,const DirectX::XMFLOAT3& up)
{
	//Create view by using eye focus up
	DirectX::XMVECTOR Eye = DirectX::XMLoadFloat3(&eye);
	DirectX::XMVECTOR Focus = DirectX::XMLoadFloat3(&focus);
	DirectX::XMVECTOR Up = DirectX::XMLoadFloat3(&up);
	DirectX::XMMATRIX View = DirectX::XMMatrixLookAtLH(Eye, Focus, Up);
	DirectX::XMStoreFloat4x4(&view, View);

	//Reverse the View matrix to get World matrix
	// w=(v)-1
	DirectX::XMMATRIX World = DirectX::XMMatrixInverse(nullptr, View);
	DirectX::XMFLOAT4X4 world;
	DirectX::XMStoreFloat4x4(&world, World);

	this->right.x = world._11;
	this->right.y = world._12;
	this->right.z = world._13;

	this->up.x = world._21;
	this->up.y = world._22;
	this->up.z = world._23;
	
	this->front.x = world._31;
	this->front.y = world._32;
	this->front.z = world._33;

	this->eye = eye;
	this->focus = focus;
}
void Camera::SetPerspectiveFov(float fovY, float aspect, float nearZ, float farZ)
{
	//Create projection by using fovY aspect nearZ farZ
	// fov = Field of View 
	// aspect = aspect ratio
	DirectX::XMMATRIX Projection = DirectX::XMMatrixPerspectiveFovLH(fovY, aspect, nearZ, farZ);
	DirectX::XMStoreFloat4x4(&projection, Projection);
}