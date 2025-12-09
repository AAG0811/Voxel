#include <cmath>
// #include <iostream>

typedef unsigned int uint32_t;

inline float Interpolate(float a, float b, float x) {
    float ft = x * 3.1415927;
    float f = (1 - cos(ft)) * .5;
    return a*(1-f) + b*f;
}
inline float Noise(int x, int y) {
    uint32_t n = x * 374761393u + y * 668265263u; 
    n = (n ^ (n >> 13)) * 1274126177u;
    return (n ^ (n >> 16)) * (1.0f/4294967295.0f);
}


inline float SmoothNoise2D(float x, float y) {
    float corners = (Noise(x-1, y-1)+Noise(x+1, y-1)+Noise(x-1, y+1)+Noise(x+1, y+1)) / 16;
    float sides = (Noise(x-1, y) +Noise(x+1, y) + Noise(x, y-1) + Noise(x, y+1)) / 8;
    float center = Noise(x, y) / 4;
    return corners + sides + center;
}

inline float InterpolateNoise(float x, float y) {
    int intx = floor(x); // !!! might need to cast rather than round
    float fractx = x - intx;

    int inty = floor(y); // !!! same as for intx
    float fracty = y - inty;

    float v1 = SmoothNoise2D(intx, inty);
    float v2 = SmoothNoise2D(intx + 1, inty);
    float v3 = SmoothNoise2D(intx, inty + 1);
    float v4 = SmoothNoise2D(intx + 1, inty + 1);

    float i1 = Interpolate(v1, v2, fractx);
    float i2 = Interpolate(v3, v4, fractx);

    return Interpolate(i1, i2, fracty);
}
class PerlinNoise{
    public:
    float persistence = 0.25; // 1/4 persistence
    int octaves = 4;
    float scale = 1.0;
    float PerlinNoise2D(float x, float y) {
        x *= scale;
        y *= scale;
        float total = 0;
        float p = persistence;
        int n = octaves;
        for (int i = 0; i < n; i++) {
            float frequency = pow(2, i);
            float amplitude = pow(p, i);
            total = total + InterpolateNoise(x * frequency, y * frequency) * amplitude;
        }
        return total;
        
    }
};