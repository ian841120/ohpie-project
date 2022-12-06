#include "GeometricPrimitive.hlsli"
VS_OUT main( float4 position : POSITION,float4 normal:NORMAL )
{
    VS_OUT vout;
    vout.position = mul(position, mul(world, view_projection));
    normal.w = 0;
    float4 N = normalize(mul(normal, world));
    float4 L = normalize(-lightDirection);
    vout.color.rgb = color.rgb * max(0, dot(L, N));
    
    vout.color.a = color.a;
    //vout.color = color;
    return vout;
    

}