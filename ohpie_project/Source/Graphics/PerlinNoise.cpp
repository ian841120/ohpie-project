#include "PerlinNoise.h"
PerlinNoise::PerlinNoise()
{
	for(int i = 0; i < 512; i++)
	{
		p.emplace_back(permutation[i % 256]);
	}
}
double PerlinNoise::Create3DNoise(double x, double y, double z)
{
	if (repeat > 0)
	{
		double xf = x - static_cast<int>(x);
		double yf = y - static_cast<int>(y);
		double zf = z - static_cast<int>(z);
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
	
	double xf = x - static_cast<int>(x);
	double yf = y - static_cast<int>(y);
	double zf = z - static_cast<int>(z);


	double u = fade(xf);
	double v = fade(yf);
	double w = fade(zf);

	int aaa, aba, aab, abb, baa, bba, bab, bbb;
	aaa = p[p[p[xi] + yi] + zi];
	aba = p[p[p[xi] + inc(yi)] + zi];
	aab = p[p[p[xi] + yi] + inc(zi)];
	abb = p[p[p[xi] + inc(yi)] + inc(zi)];
	baa = p[p[p[inc(xi)] + yi] + zi];
	bba = p[p[p[inc(xi)] + inc(yi)] + zi];
	bab = p[p[p[inc(xi)] + yi] + inc(zi)];
	bbb = p[p[p[inc(xi)] + inc(yi)] + inc(zi)];

	double x1, x2, y1, y2;
	x1 = lerp(grad(aaa, xf, yf, zf), grad(baa, xf - 1, yf, zf), u);
	x2 = lerp(grad(aba, xf, yf-1, zf), grad(bba, xf - 1, yf - 1, zf), u);
	y1 = lerp(x1, x2, v);
	x1 = lerp(grad(aab, xf, yf, zf - 1), grad(abb, xf - 1, yf, zf - 1), u);
	x2 = lerp(grad(abb, xf, yf - 1, zf - 1), grad(bbb, xf - 1, yf - 1, zf - 1), u);
	y2 = lerp(x1, x2, v);
	return (lerp(y1, y2, w) + 1) / 2;
}


double PerlinNoise::fade(double t)
{
	//fade function = 6t^5-15t^4+10t^3
	return t * t * t * (t * (t * 6 - 15) + 10);
}
double PerlinNoise::lerp(double a, double b, double t)
{
	return a + t * (b - a);
}
int PerlinNoise::inc(int num)
{
	num++;
	if (repeat > 0)num %= repeat;
	return num;
}
double PerlinNoise::grad(int hash, double x, double y, double z)
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
double PerlinNoise::OctavePerlin(double x, double y, double z, int octaves, double persistence)
{
	double total = 0;
	double frequency = 1;
	double amplitude = 1;
	double maxValue = 0;
	for (int i = 0; i < octaves; i++)
	{
		total += Create3DNoise(x * frequency, y * frequency, y * frequency) * amplitude;
		maxValue += amplitude;
		amplitude *= persistence;
		frequency *= 2;
	}
	return total / maxValue;
}