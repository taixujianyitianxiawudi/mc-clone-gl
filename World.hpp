#ifndef WORLD_HPP
#define WORLD_HPP

#include <vector>
#include <array>
#include "settings.hpp"
#include "voxel_handler.hpp"
#include "world_objects/Chunk.hpp"


class VoxelEngine;  // Forward declaration of App class if needed
class VoxelHandler;

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
    std::array<std::array<float, CHUNK_VOL>, WORLD_VOL> voxels_world;
    std::array<Chunk*, WORLD_VOL> chunks;
    Chunk* getChunkAt(const glm::ivec3& chunkPos) const;
    VoxelHandler* voxel_handler;
private:
    VoxelEngine* app;

};

#endif  // WORLD_HPP