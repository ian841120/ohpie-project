#pragma once
#include <directxmath.h>
class Camera
{
private:
	Camera(){}
	~Camera(){}
public:
	// Singleton
	static Camera& Instance()
	{
		static Camera camera;
		return camera;
	}
	void SetLookAt(const DirectX::XMFLOAT3& eye, const DirectX::XMFLOAT3& focus, const DirectX::XMFLOAT3& up);
	void SetPerspectiveFov(float fovY, float aspect, float nearZ, float farZ);
	const DirectX::XMFLOAT3& GetEye()const { return eye; }
	const DirectX::XMFLOAT3& GetFocus()const { return focus; }
	const DirectX::XMFLOAT3& GetUp()const { return up; }
	const DirectX::XMFLOAT3& GetFront()const { return front; }
	const DirectX::XMFLOAT3& GetRight()const { return right; }
	const DirectX::XMFLOAT4X4& GetView()const { return view; }
	const DirectX::XMFLOAT4X4& GetProjection() const{ return projection; }
private:
	DirectX::XMFLOAT3	eye{ 0,0,10 };
	DirectX::XMFLOAT3	focus{ 0,0,0 };
	
	DirectX::XMFLOAT3	up{ 0,1,0 };
	DirectX::XMFLOAT3	front{ 0,0,1 };
	DirectX::XMFLOAT3	right{ 1,0,0 };

	DirectX::XMFLOAT4X4 view{ 1,0,0,0, 
							  0,1,0,0,
							  0,0,1,0,
							  0,0,0,1 };
	DirectX::XMFLOAT4X4 projection{ 1,0,0,0,
									0,1,0,0,
									0,0,1,0,
									0,0,0,1 };
};