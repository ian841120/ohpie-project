#include "GeometricPrimitive.hlsli"
float4 main(VS_OUT pin) : SV_TARGET
{
    float3 toEye = viewPosition.xyz - pin.world_position.xyz;
    float distToEye = length(toEye);
    float3 N = normalize(pin.normal);
    float3 L = normalize(directionalLightData.direction.xyz);
    float3 kd = float3(1, 1, 1);
    
    float3 directionalDiffuse = CalcLambertDiffuse(N, L, directionalLightData.color.rgb, kd);
    
    float3 pointDiffuse = (float3) 0;
    for (int i = 0; i < pointLightCount;i++)
    {
        float3 lightVector = pin.world_position.xyz - pointLightData[i].position.xyz;
        float lightLength = length(lightVector);
        if (lightLength>pointLightData[i].range)
            continue;
        float attenuate = clamp(1.0f - (lightLength / pointLightData[i].range), 0.0f, 1.0f);
        lightVector = normalize(lightVector);
        pointDiffuse += CalcLambertDiffuse(N, lightVector, pointLightData[i].color.rgb, kd) * attenuate;

    }

    float4 color = pin.color;
    color.rgb *= (directionalDiffuse + pointDiffuse);

    //Fog
    {
        float fogLerp = saturate((distToEye - fogData.fogStart) / fogData.fogRange);
        color.rgb = lerp(color.rgb, fogData.fogColor, fogLerp);
    }

    return color;
}