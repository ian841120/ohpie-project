#include "PerlinNoise.h"
PerlinNoise::PerlinNoise()
{
	for(int i = 0; i < 512; i++)
	{
		p.emplace_back(permutation[i % 256]);
	}
}
float PerlinNoise::CreateNoise(float x, float y, float z)
{
	if (repeat > 0)
	{
		float xf = x - static_cast<int>(x);
		float yf = y - static_cast<int>(y);
		float zf = z - static_cast<int>(z);
		x = static_cast<int>(x) % repeat;
		y = static_cast<int>(y) % repeat;
		z = static_cast<int>(z) % repeat;
		x += xf;
		y += yf;
		z += zf;
	}

	int xi = (int)x & 255;
	int yi = (int)y & 255;
	int zi = (int)z & 255;
	
	float xf = x - static_cast<int>(x);
	float yf = y - static_cast<int>(y);
	float zf = z - static_cast<int>(z);


	float u = fade(xf);
	float v = fade(yf);
	float w = fade(zf);

	int aaa, aba, aab, abb, baa, bba, bab, bbb;
	aaa = p[(p[(p[xi & 255] + yi) & 255] + zi) & 255];
	aba = p[(p[(p[xi & 255] + inc(yi)) & 255] + zi) & 255];
	aab = p[(p[(p[xi & 255] + yi) & 255] + inc(zi)) & 255];
	abb = p[(p[(p[xi & 255] + inc(yi)) & 255] + inc(zi)) & 255];
	baa = p[(p[(p[inc(xi) & 255] + yi) & 255] + zi) & 255];
	bba = p[(p[(p[inc(xi) & 255] + inc(yi)) & 255] + zi) & 255];
	bab = p[(p[(p[inc(xi) & 255] + yi) & 255] + inc(zi)) & 255];
	bbb = p[(p[(p[inc(xi) & 255] + inc(yi)) & 255] + inc(zi)) & 255];

	float x1, x2, y1, y2;
	x1 = lerp(grad(aaa, xf, yf, zf), grad(baa, xf - 1, yf, zf), u);
	x2 = lerp(grad(aba, xf, yf-1, zf), grad(bba, xf - 1, yf - 1, zf), u);
	y1 = lerp(x1, x2, v);
	x1 = lerp(grad(aab, xf, yf, zf - 1), grad(bab, xf - 1, yf, zf - 1), u);
	x2 = lerp(grad(abb, xf, yf - 1, zf - 1), grad(bbb, xf - 1, yf - 1, zf - 1), u);
	y2 = lerp(x1, x2, v);
	return (lerp(y1, y2, w) + 1) / 2;
}
float PerlinNoise::CreateNoise(float x, float y)
{

	int x1 = int(x)&255;
	int y1 = int(y)&255;
	int x2 = x1 + 1;
	int y2 = y1 + 1;

	float fractX = x - int(x);
	float fractY = y - int(y);

	float u = fade(fractX);
	float v = fade(fractY);
	

	int aa, ab, ba, bb;
	aa = p[(p[x1&255] + y1)&255];
	ab = p[(p[x1&255] + y2)&255];
	ba = p[(p[x2&255] + y1)&255];
	bb = p[(p[x2&255] + y2)&255];


	float lerp1 = lerp(grad(aa, fractX, fractY), grad(ba, fractX - 1, fractY), u);
	float lerp2 = lerp(grad(ab, fractX, fractY - 1), grad(bb, fractX - 1, fractY - 1), u);
	return (lerp(lerp1, lerp2, v));
}
float PerlinNoise::fade(float t)
{
	//fade function = 6t^5-15t^4+10t^3
	return t * t * t * (t * (t * 6 - 15) + 10);
}
float PerlinNoise::lerp(float a, float b, float t)
{
	return a + t * (b - a);
}
int PerlinNoise::inc(int num)
{
	num++;
	if (repeat > 0)num %= repeat;
	return num;
}
float PerlinNoise::grad(int hash, float x, float y)
{
  int h = hash & 15;
  float u = h<8 ? x : y;
  float v = h<4 ? y : h==12||h==14 ? x : 0;
  return ((h&1) == 0 ? u : -u) + ((h&2) == 0 ? v : -v);}
float PerlinNoise::grad(int hash, float x, float y, float z)
{
	switch (hash & 0xF)
	{
	case 0x0: return  x + y;
	case 0x1: return -x + y;
	case 0x2: return  x - y;
	case 0x3: return -x - y;
	case 0x4: return  x + z;
	case 0x5: return -x + z;
	case 0x6: return  x - z;
	case 0x7: return -x - z;
	case 0x8: return  y + z;
	case 0x9: return -y + z;
	case 0xA: return  y - z;
	case 0xB: return -y - z;
	case 0xC: return  y + x;
	case 0xD: return -y + z;
	case 0xE: return  y - x;
	case 0xF: return -y - z;
	default: return 0; 

	}
}
float PerlinNoise::OctavePerlin(float x, float y, float z, int octaves, float persistence)
{
	float total = 0;
	float frequency = 1;
	float amplitude = 1;
	float maxValue = 0;
	for (int i = 0; i < octaves; i++)
	{
		total += CreateNoise(x * frequency, y * frequency, z * frequency) * amplitude;
		maxValue += amplitude;
		amplitude *= persistence;
		frequency *= 2;
		
		
	}
	return total / maxValue;
}
float PerlinNoise::OctavePerlin(float x, float y, int octaves, float persistence)
{
	float total = 0;
	float frequency = 1;
	float amplitude = 1;
	float maxValue = 0;
	for (int i = 0; i < octaves; i++)
	{
		total += CreateNoise(x * frequency, y * frequency) * amplitude;
		maxValue += amplitude;
		amplitude *= persistence;
		frequency *= 2;


	}
	return total / maxValue;

}