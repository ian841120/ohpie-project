#include "Terrain.hlsli"
float4 main(VS_OUT pin) : SV_TARGET
{
    return float4(1.0f * pin.world_position.y, 1.0f * pin.world_position.y, 1.0f * pin.world_position.y, 1.0f);
}