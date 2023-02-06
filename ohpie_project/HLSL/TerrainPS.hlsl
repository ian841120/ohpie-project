#include "Terrain.hlsli"
float4 main(VS_OUT pin) : SV_TARGET
{
    //pin.world_position = normalize(pin.world_position);
    if (pin.world_position.y<0.01)
        return float4(0.0f, 0.0f, 1.0f, 1.0f);
    return float4(0.0f, 1.0f, 0.0f, 1.0f);

}