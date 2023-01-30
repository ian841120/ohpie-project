struct VS_OUT
{
    float4 position : SV_POSITION;
};
cbuffer CBuffer
{
    row_major float4x4 view_projection;
};