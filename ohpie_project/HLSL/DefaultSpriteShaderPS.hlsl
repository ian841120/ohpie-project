#include "DefaultSprite.hlsli"
Texture2D texture2d : register(t0);
SamplerState samplerState : register(s0);

float4 main(VS_OUT pin) : SV_TARGET
{
    return texture2d.Sample(samplerState, pin.texcoord) * pin.color;

}