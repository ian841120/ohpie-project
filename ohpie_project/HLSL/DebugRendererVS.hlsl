#include "DebugRenderer.hlsli"

VS_OUT main(float4 position:POSITION)
{
    VS_OUT vout;
    vout.position = mul(position, WorldViewProjection);
    vout.color = color;
    return vout;

}