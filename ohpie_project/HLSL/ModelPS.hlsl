#include "Model.hlsli"
Texture2D texture2d;
SamplerState sample;
float4 main(VS_OUT pin) : SV_TARGET
{
    float4 color = texture2d.Sample(sample, pin.texcoord);
    return color;
}