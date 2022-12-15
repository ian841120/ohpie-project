#pragma once
#include <d3d11.h>
#include <memory>
void create_vs_from_file(ID3D11Device* device, const char* filename, ID3D11VertexShader** vertexShader, ID3D11InputLayout** inputLayout, D3D11_INPUT_ELEMENT_DESC* inputDesc, int inputCount);
void create_ps_from_file(ID3D11Device* device, const char* filename, ID3D11PixelShader** pixelShader);
