#pragma once



class Noise {
public:
    Noise(int seed);
    ~Noise();

    // 2D noise function
    float noise2(float x, float y);

    // 3D noise function
    float noise3(float x, float y, float z);

private:
    // OpenSimplex noise implementation (or use FastNoise)
    class OpenSimplexNoise* simplexNoise;
};

extern Noise noise;

