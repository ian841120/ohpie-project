#include "GeometricPrimitive.hlsli"
VS_OUT main( float4 position : POSITION,float4 normal:NORMAL )
{
    VS_OUT vout;
    vout.position = mul(position, mul(world, view_projection));
    vout.normal = normalize(mul(float4(normal.xyz, 0.0f),world));
    vout.world_position = mul(position, world);
    vout.color = color;
    return vout;
    

}