struct DirectionalLightData
{
    float4 direction; // Œü‚«
    float4 color; // F
};

float3 CalcLambertDiffuse(float3 normal,float3 lightVector,float3 lightColor,float3 kd)
{
    float d = max(0, dot(-lightVector, normal));
    return d * lightColor * kd;
}