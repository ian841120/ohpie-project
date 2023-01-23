#include "MaskShader.hlsli"
Texture2D texture0 : register(t0);
SamplerState sampler0:register(s0);

Texture2D maskTexture : register(t1);

float4 main(VS_OUT pin):SV_TARGET
{
    float4 color = texture0.Sample(sampler0, pin.texcoord) * pin.color;
    
    float mask = maskTexture.Sample(sampler0, pin.texcoord).r;
    float alpha = step(mask, dissolveThreshold);
    float edgeValue = step(mask - dissolveThreshold, dissolveThreshold) * step(dissolveThreshold, mask) * step(mask, dissolveThreshold + edgeThreshold);

    color.rgb += edgeColor.rgb * edgeValue;
    alpha = saturate(alpha + edgeValue);
    
    color.a *= alpha;
    clip(color.a - 0.01f);
    return color;

}
