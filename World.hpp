#ifndef WORLD_HPP
#define WORLD_HPP

#include <vector>
#include <array>
#include "settings.hpp"
#include "world_objects/Chunk.hpp"


class VoxelEngine;  // Forward declaration of App class if needed

class World {
public:
    // Constructor
    World(VoxelEngine* app);

    // Build functions
    void buildChunks();
    void buildChunkMesh();

    // Game loop functions
    void update();
    void render();
    VoxelEngine* getApp();
    std::array<std::array<int, CHUNK_VOL>, WORLD_VOL> voxels;
    std::array<Chunk*, WORLD_VOL> chunks;
private:
    VoxelEngine* app;

};

#endif  // WORLD_HPP