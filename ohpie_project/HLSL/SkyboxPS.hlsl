#include "Skybox.hlsli"
TextureCube CubeMap : register(t0);
SamplerState Sample : register(s0);

float4 main(VS_OUT pin) : SV_TARGET
{
    return CubeMap.Sample(Sample, pin.Texture);
}