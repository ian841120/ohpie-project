#include "Texture.h"
#include "Graphics.h"
#include "WICTextureLoader.h"
#include <string>
Texture::Texture(const char* filename)
{
	ID3D11Device* device = Graphics::GetInstance().GetDevice();
	HRESULT hr;

	std::string str = filename;
	textureName = str.substr(str.find_last_of("/\\"));
	textureName.erase(0,1);

	std::wstring wfilename= std::wstring(str.begin(), str.end());
	Microsoft::WRL::ComPtr<ID3D11Resource> resource;
	hr = DirectX::CreateWICTextureFromFile(device, wfilename.c_str(), resource.GetAddressOf(), srv.ReleaseAndGetAddressOf());
	if (SUCCEEDED(hr))
	{
		Microsoft::WRL::ComPtr<ID3D11Texture2D>texture2d;
		hr = resource.Get()->QueryInterface<ID3D11Texture2D>(texture2d.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), L"FAIL");
		texture2d->GetDesc(&texture2dDesc);
	}
}
