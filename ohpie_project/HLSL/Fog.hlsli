struct FogData
{
	float4 fogColor;
	float fogStart;
	float fogRange;
	float2 padding;
};
float3 calcFogColor(float distToEye, float fogStart, float fogRange, float3 fogColor, float3 color, float fogDensity=1.0f)
{
    float fogFactor = saturate((distToEye - fogStart) / fogRange * fogDensity);
    return fogFactor * (fogColor - color);
}