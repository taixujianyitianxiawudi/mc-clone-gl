#include "terrain_gen.hpp"
#include "noise.hpp" // Assuming you have a noise generation library
#include <algorithm> // For std::max

Noise noise(SEED);

// Generate terrain height based on noise and island shape
int getHeight(int x, int z) {
    // Island mask
    float distance = std::hypot(x - CENTER_XZ, z - CENTER_XZ);
    float island = 1.0f / (std::pow(0.0025f * distance, 20) + 0.0001f);
    island = std::min(island, 1.0f);

    // Amplitudes
    float a1 = CENTER_Y;
    float a2 = a1 * 0.5f, a4 = a1 * 0.25f, a8 = a1 * 0.125f;

    // Frequencies
    float f1 = 0.005f;
    float f2 = f1 * 2.0f, f4 = f1 * 4.0f, f8 = f1 * 8.0f;

    // Height calculation
    float height = 0.0f;
    if (noise.noise2(0.1f * x, 0.1f * z) < 0) {
        a1 /= 1.07f;
    }

    height += noise.noise2(x * f1, z * f1) * a1 + a1;
    height += noise.noise2(x * f2, z * f2) * a2 - a2;
    height += noise.noise2(x * f4, z * f4) * a4 + a4;
    height += noise.noise2(x * f8, z * f8) * a8 - a8;

    height = std::max(height, noise.noise2(x * f8, z * f8) + 2.0f);
    height *= island;

    return static_cast<int>(height);
}

// Get the index of a voxel in the chunk
int getIndex(int x, int y, int z) {
    return x + CHUNK_SIZE * z + CHUNK_AREA * y;
}

// Set the voxel ID based on its position and world height
void setVoxelID(std::array<float, CHUNK_VOL>& voxels, int x, int y, int z, int wx, int wy, int wz, int worldHeight) {
    int voxelID = 0;

    if (wy < worldHeight - 1) {
        // Create caves using 3D noise
        if (noise.noise3(wx * 0.09f, wy * 0.09f, wz * 0.09f) > 0 &&
            noise.noise2(wx * 0.1f, wz * 0.1f) * 3 + 3 < wy && wy < worldHeight - 10) {
            voxelID = 0;
        } else {
            voxelID = STONE;
        }
    } else {
        int rng = static_cast<int>(7 * static_cast<float>(std::rand()) / RAND_MAX);
        int ry = wy - rng;

        if (ry >= SNOW_LVL && ry < worldHeight) {
            voxelID = SNOW;
        } else if (ry >= STONE_LVL && ry < SNOW_LVL) {
            voxelID = STONE;
        } else if (ry >= DIRT_LVL && ry < STONE_LVL) {
            voxelID = DIRT;
        } else if (ry >= GRASS_LVL && ry < DIRT_LVL) {
            voxelID = GRASS;
        } else {
            voxelID = SAND;
        }
    }

    // Set voxel ID
    voxels[getIndex(x, y, z)] = voxelID;

    // Place tree if applicable
    if (wy < DIRT_LVL) {
        placeTree(voxels, x, y, z, voxelID);
    }
}

// Place a tree in the world
void placeTree(std::array<float, CHUNK_VOL>& voxels, int x, int y, int z, int voxelID) {
    float rnd = static_cast<float>(std::rand()) / RAND_MAX;
    if (voxelID != GRASS || rnd > TREE_PROBABILITY) {
        return;
    }
    if (y + TREE_HEIGHT >= CHUNK_SIZE || x - TREE_H_WIDTH < 0 || x + TREE_H_WIDTH >= CHUNK_SIZE || z - TREE_H_WIDTH < 0 || z + TREE_H_WIDTH >= CHUNK_SIZE) {
        return;
    }

    // Dirt under the tree
    voxels[getIndex(x, y, z)] = DIRT;

    // Leaves
    int m = 0;
    for (int n = 0, iy = TREE_H_HEIGHT; iy < TREE_HEIGHT - 1; ++n, ++iy) {
        int k = iy % 2;
        int rng = static_cast<int>(std::rand()) % 2;
        for (int ix = -TREE_H_WIDTH + m; ix < TREE_H_WIDTH - m * rng; ++ix) {
            for (int iz = -TREE_H_WIDTH + m * rng; iz < TREE_H_WIDTH - m; ++iz) {
                if ((ix + iz) % 4) {
                    voxels[getIndex(x + ix + k, y + iy, z + iz + k)] = LEAVES;
                }
            }
        }
        m += (n > 0) ? 1 : (n > 1) ? 3 : 0;
    }

    // Tree trunk
    for (int iy = 1; iy < TREE_HEIGHT - 2; ++iy) {
        voxels[getIndex(x, y + iy, z)] = WOOD;
    }

    // Tree top
    voxels[getIndex(x, y + TREE_HEIGHT - 2, z)] = LEAVES;
}
