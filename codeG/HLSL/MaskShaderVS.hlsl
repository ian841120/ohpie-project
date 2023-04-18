#include "MaskShader.hlsli"

VS_OUT main(float4 position:POSITION,float4 color:COLOR,float2 texcoord:TEXCOORD)
{
    VS_OUT vout;
    vout.color = color;
    vout.position = position;
    vout.texcoord = texcoord;
    
    return vout;
}