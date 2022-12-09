#include "GeometricPrimitive.hlsli"
float4 main(VS_OUT pin) : SV_TARGET
{
    float3 N = normalize(pin.normal);
    float3 L = normalize(directionalLightData.direction.xyz);
    float3 kd = float3(1, 1, 1);
    float3 directionalDiffuse = CalcLambertDiffuse(N, L, directionalLightData.color.rgb, kd);
    float4 color = pin.color;
    color.rgb *= directionalDiffuse;
    return color  ;
}