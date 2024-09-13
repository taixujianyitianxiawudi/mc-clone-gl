#ifndef TERRAIN_GEN_HPP
#define TERRAIN_GEN_HPP

#include <array>
#include <cmath>
#include <random>
#include "settings.hpp"

// Function to get the height at a given (x, z) position
int getHeight(int x, int z);

// Function to get the index of a voxel in the chunk
int getIndex(int x, int y, int z);

// Function to set the voxel ID based on its position and world height
void setVoxelID(std::array<float, CHUNK_VOL>& voxels, int x, int y, int z, int wx, int wy, int wz, int worldHeight);

// Function to place a tree in the world
void placeTree(std::array<float, CHUNK_VOL>& voxels, int x, int y, int z, int voxelID);

#endif // TERRAIN_GEN_HPP