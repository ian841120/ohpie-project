struct DirectionalLightData
{
    float4 direction; // å¸Ç´
    float4 color; // êF
};
struct PointLightData
{
    float4 position;
    float4 color;
    float range;
    float3 dummy;
};
static const int pointLightMax = 8;
struct SpotLightData
{
    float4 position;
    float4 direction;
    float4 color;
    float range;
    float innerCone;
    float outerCone;
    float dummy;
};
static const int spotLightMax = 8;
float3 CalcLambertDiffuse(float3 normal,float3 lightVector,float3 lightColor,float3 kd)
{
    float d = max(0, dot(-lightVector, normal));
    return d * lightColor * kd;
}