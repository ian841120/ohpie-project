struct VS_OUT
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
    float2 texcoord : TEXCOORD;
};
cbuffer CbMask : register(b0)
{
    float dissolveThreshold;
    float edgeThreshold;
    float2 path;
    float4 edgeColor;

}