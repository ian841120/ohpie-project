#include "Model.hlsli"
VS_OUT main(float4 position : POSITION, float4 normal : NORMAL,float2 texcoord:TEXCOORD)
{
    VS_OUT vout;
    vout.position = mul(position, view_projection);
    vout.normal = normalize(float4(normal.xyz, 0.0f));
    vout.texcoord = texcoord;
    return vout;
    

}