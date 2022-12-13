struct DirectionalLightData
{
    float4 direction; // Œü‚«
    float4 color; // F
};

float3 CalcLambertDiffuse(float3 normal,float3 lightVector,float3 lightColor,float3 kd)
{
    float d = dot(-lightVector, normal) * 0.5 + 0.5;
    return d * lightColor * kd;
}