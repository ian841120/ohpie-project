#include "Model.hlsli"
VS_OUT main(float4 position : POSITION, float4 normal : NORMAL)
{
    VS_OUT vout;
    vout.position = mul(position, view_projection);
    vout.normal = normalize(float4(normal.xyz, 0.0f));
    return vout;
    

}