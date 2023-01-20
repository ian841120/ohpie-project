#pragma once
#include <d3d11.h>
#include <wrl.h>
class Texture
{
public:
	Texture(const char* filename);
	~Texture() {};
	const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& GetSRV() const { return srv; } //GetShaderResourceView	
	int GetWidth()const { return texture2dDesc.Width; }
	int GetHeight()const { return texture2dDesc.Height; }
private:
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv;
	// texture description
	D3D11_TEXTURE2D_DESC texture2dDesc;
};