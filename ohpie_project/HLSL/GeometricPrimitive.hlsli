struct VS_OUT
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};
cbuffer Cbuffer : register(b0)
{
    row_major float4x4  world;
    row_major float4x4  view_projection;
    float4              color;
    float4              lightDirection;
}