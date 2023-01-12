struct VS_OUT
{
    float4 position : SV_POSITION;
    float3 normal : NORMAL;
    float2 texcoord : TEXCOORD;
};
cbuffer Cbuffer : register(b0)
{
    row_major float4x4 view_projection;

}