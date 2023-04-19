#pragma once
#include <d3d11.h>
#include<directxmath.h>
#include<wrl.h>
#include "RenderStates.h"
class Sprite
{
public:
	Sprite();
	~Sprite() {};
	enum class POVIT
	{
		CENTER,
		UPPER_LEFT,
		UPPER_RIGHT,
		LOWER_LEFT,
		LOWER_RIGHT,

		CENTER_UPPER,
		CENTER_LOWER,
		CENTER_RIGHT,
		CENTER_LEFT,
	};
	struct Vertex
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT4 color;
		DirectX::XMFLOAT2 texcoord;
	};
private:
	
	Microsoft::WRL::ComPtr<ID3D11Buffer>				vertex_buffer;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	shader_resource_view;
	D3D11_TEXTURE2D_DESC								texture2d_desc;

	int textureWidth = 0;
	int textureHeight = 0;
	POVIT povit = POVIT::CENTER;

public:
	const Microsoft::WRL::ComPtr<ID3D11Buffer>& GetVertexBuffer()const { return vertex_buffer; }
	const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetShaderResourceView()const { return shader_resource_view; }
	void SetPovit(POVIT p) { povit = p; }
	DirectX::XMFLOAT2 calcPovit(float dx, float dy, float dw, float dh);
	void SetShaderResourceView(const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& srv, int texWidth, int texHeight);
	void Update(float dx, float dy, float dw, float dh);
	void Update(float dx, float dy, float dw, float dh, float r, float g , float b, float a , float angle );
	void Update(float dx, float dy, float dw, float dh, float r, float g , float b , float a , float angle , float sx , float sy , float sw , float sh );
};