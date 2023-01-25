#include "AtmosphericShader.hlsli"
VS_OUT main( float4 position : POSITION )
{
    VS_OUT vout;
    vout.position = position;
    return vout;
}