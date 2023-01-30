#include "Terrain.hlsli"
VS_OUT main( float4 position : POSITION )
{
    VS_OUT vout;
    vout.position = mul(position, view_projection);
    return vout;

}