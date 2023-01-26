#include "AtmosphericShader.hlsli"
VS_OUT main( float4 position : POSITION )
{
    VS_OUT vout;
    vout.position = mul(position, mul(world, view_project));
    vout.worldPosition = mul(position, world);
    return vout;
}