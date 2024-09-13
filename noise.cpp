
#include "noise.hpp"
#include "deps/OpenSimplexNoise.hpp"

Noise::Noise(int seed) {
    // Initialize OpenSimplex noise with the given seed
    simplexNoise = new OpenSimplexNoise(seed);
}

Noise::~Noise() {
    delete simplexNoise;
}

// 2D noise function
float Noise::noise2(float x, float y) {
    return simplexNoise->eval(x, y);
}

// 3D noise function
float Noise::noise3(float x, float y, float z) {
    return simplexNoise->eval(x, y, z);
}