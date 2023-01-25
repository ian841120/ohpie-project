#include "AtmosphericShader.hlsli"
float4 main(VS_OUT pin) : SV_TARGET
{
    float3 worldPos = pin.position.xyz;
    worldPos = IntersectionPos(normalize(worldPos), float3(0.0, innerRadius, 0.0), outerRadius);
    float3 cameraPos = float3(0.0, innerRadius, 0.0);
    float3 lightDir = normalize(directionalLightData.direction.xyz);
    float3 ray = worldPos - cameraPos;
    float far = length(ray);
    ray /= far;
    
    float3 start = cameraPos;
    float cameraHeight = length(cameraPos);
    float startAngle = dot(ray, start)/ cameraHeight;
    float startDepth = exp(scaleOverScaleDepth * (innerRadius - cameraHeight));
    float startOffset = startDepth * IntegralApproximation(startAngle);
    
    float sampleLength = far / samples;
    float scaledLength = sampleLength * scale;
    float3 sampleRay = ray * sampleLength;
    float3 samplePoint = start + sampleRay * 0.5f;

    float3 frontColor = float3(0.0, 0.0, 0.0);
    for (int i = 0; i < (int)samples; i++)
    {
        float height = length(samplePoint);
        float depth = exp(scaleOverScaleDepth * (innerRadius - height));
        float lightAngle = dot(lightDir, samplePoint) / height;
        float cameraAngle = dot(ray, samplePoint) / height;
        float scatter = (startOffset + depth * (IntegralApproximation(lightAngle) - IntegralApproximation(cameraAngle)));
        float3 attenuate = exp(-scatter * (invWaveLength * kr4PI + km4PI));
        frontColor += attenuate * (depth * scaledLength);
        samplePoint += sampleRay;
    }

    float3 c0 = frontColor * (invWaveLength * krESun);
    float3 c1 = frontColor * kmESun;
    float3 direction = cameraPos - worldPos;

    float cos = dot(lightDir, direction) / length(direction);
    float cos2 = cos * cos;
    //return float4(1.0, 1.0, 1.0, 1.0);
    float miePhase = (3 * (1 - g2) * (1 + cos2)) / (2 * (2 + g2) * pow(abs((1 + g2 - 2 * g2 * cos2)), 1.5));
    //rayPhase---> when g=0;
    float rayLeighPhase = 0.75 * (1 + cos2);

    float3 sky = c0 * rayLeighPhase + c1 * miePhase;
    return float4(directionalLightData.color.xyz * sky, directionalLightData.color.a);
}