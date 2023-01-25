#include "AtmosphericShader.hlsli"
float4 main(VS_OUT pin) : SV_TARGET
{
    float4 worldPos = pin.position;
    float4 cameraPos = worldCameraPos;
    cameraPos.y = innerRadius;
    float4 lightDir = lightdirection;
    float4 ray = worldPos - cameraPos;
    float far = length(ray);
    ray /= far;
    
    float4 start = cameraPos;
    float cameraHeight = length(cameraPos);
    float startAngle = dot(ray, start);
    float startDepth = exp(scaleOverScaleDepth * (innerRadius - cameraHeight));
    float startoffset = startDepth * IntegralApproximation(startAngle);
    
    return float4(1.0, 1.0, 1.0, 1.0);
    
    //float cos2 = cos * cos;
    //float miePhase = (3 * (1 - g2) * (1 + cos2)) / (2 * (2 + g2) * pow(abs((1 + g2 - 2 * g2 * cos2)), 1.5));
    ////rayPhase---> when g=0;
    //float rayPhase = 0.75 * (1 + cos2);

}