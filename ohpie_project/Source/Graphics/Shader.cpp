#include "Shader.h"
void create_vs_from_file(ID3D11Device* device, const char* filename, ID3D11VertexShader** vertexShader, ID3D11InputLayout** inputLayout, D3D11_INPUT_ELEMENT_DESC* inputDesc, int inputCount)
{
	HRESULT hr = { S_OK };
	// Read cso file
	FILE* fp{ nullptr };
	fopen_s(&fp, filename, "rb");
	_ASSERT_EXPR(fp, L"VS CSO File not found");
	fseek(fp, 0, SEEK_END);
	long cso_sz{ ftell(fp) };
	fseek(fp, 0, SEEK_SET);
	std::unique_ptr<unsigned char[]>cso_data{ std::make_unique<unsigned char[]>(cso_sz) };
	fread(cso_data.get(), cso_sz, 1, fp);
	fclose(fp);
	//Create vertex shader
	hr = device->CreateVertexShader(cso_data.get(), cso_sz, nullptr, vertexShader);
	_ASSERT_EXPR(SUCCEEDED(hr), L"Failed to create vertexshader");
	//Create input layout
	hr = device->CreateInputLayout(inputDesc, inputCount, cso_data.get(), cso_sz, inputLayout);
	_ASSERT_EXPR(SUCCEEDED(hr), L"Failed to create inputlayout");

}
void create_ps_from_file(ID3D11Device* device, const char* filename, ID3D11PixelShader** pixelShader)
{
	HRESULT hr = { S_OK };
	// Read cso file
	FILE* fp{ nullptr };
	fopen_s(&fp, filename, "rb");
	_ASSERT_EXPR(fp, L"VS CSO File not found");
	fseek(fp, 0, SEEK_END);
	long cso_sz{ ftell(fp) };
	fseek(fp, 0, SEEK_SET);
	std::unique_ptr<unsigned char[]>cso_data{ std::make_unique<unsigned char[]>(cso_sz) };
	fread(cso_data.get(), cso_sz, 1, fp);
	fclose(fp);
	//Create vertex shader
	hr = device->CreatePixelShader(cso_data.get(), cso_sz, nullptr, pixelShader);
	_ASSERT_EXPR(SUCCEEDED(hr), L"Failed to create vertexshader");
	//Create input layout

}