struct VS_OUT
{
    float4 position : SV_POSITION;
    float4 world_position : POSITION;
};
cbuffer CBuffer
{
    row_major float4x4 view_projection;
};