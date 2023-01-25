#define OUTER_RADIUS 1.025  // The radius from center to top 
static const float PI = 3.14159265f;

static const float outerRadius = OUTER_RADIUS;
static const float outerRadius2 = OUTER_RADIUS * OUTER_RADIUS;
static const float innerRadius = 1.0;
static const float innerRadius2 = 1.0;

static const float scaleDepth = 0.25;
static const float scale = 1.0 / (outerRadius - innerRadius);
static const float scaleOverScaleDepth = scale / scaleDepth;

static const float km = 0.001;  //MieScatteringCoefficient
static const float kr = 0.0025; //RayleighScatteringCoefficient

static const float ESun = 20.0;   //Sun brightness
static const float kmESun = km * ESun;
static const float krESun = kr * ESun;
static const float kr4PI = 4.0 * kr * PI;
static const float km4PI = 4.0 * km * PI;


static const float g = -0.999f;
static const float g2 = g * g;


static const float3 three_primary_color = float3(0.68, 0.55, 0.44); //wavelength of light
static const float3 invWaveLength = 1.0 / pow(three_primary_color, 4.0);


static const int samples = 2;

struct DirectionalLightData
{
    float4 direction; 
    float4 color; 
};


struct VS_OUT
{
    float4 position : SV_POSITION;
};
cbuffer CbSky
{
    row_major float4x4 world;
    row_major float4x4 view_project;
    DirectionalLightData directionalLightData;

};
//When scale depth is 0.25 and the ratio of atmosphere thick to center radius is 2.5%
//we can use the follow function to find the approach value
float IntegralApproximation(float cos) 
{
    float x = 1.0 - cos;
    return scaleDepth * exp(-0.00287 + x * (0.459 + x * (3.83 + x * (-6.80 + x * 5.25))));
}

float3 IntersectionPos(float3 dir, float3 a, float radius)
{
    float b = dot(a, dir);
    float c = dot(a, a) - radius * radius;
    float d = max(b * b - c, 0.0);

    return a + dir * (-b + sqrt(d));
}
