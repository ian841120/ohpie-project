#include "Terrain.hlsli"
VS_OUT main( float4 position : POSITION)
{
    VS_OUT vout;
    vout.world_position = position;
    vout.position = mul(position, view_projection);
    return vout;

}