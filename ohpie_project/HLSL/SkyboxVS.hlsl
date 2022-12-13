#include "Skybox.hlsli"
VS_OUT main( float4 position : POSITION )
{
    VS_OUT vout;
    vout.position = mul(position, mul(world, view_projection)).xyww;
    vout.Texture = position.xyz;
    return vout;

}