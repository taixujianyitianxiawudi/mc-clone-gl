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
    void loadChunks();

    // Game loop functions
    void update();
    void render();

    void render_with_shader_program(GLuint program);

    VoxelEngine* getApp();
    std::array<std::array<float, CHUNK_VOL>, WORLD_VOL> voxels_world;
    std::array<Chunk*, WORLD_VOL> chunks;
    Chunk* getChunkAt(const glm::ivec3& chunkPos) const;
    VoxelHandler* voxel_handler;

    void saveChunkToDisk(std::array<std::array<float, CHUNK_VOL>, WORLD_VOL> &voxels_world, const std::string &filename);
    void loadChunkFromDisk(std::array<std::array<float, CHUNK_VOL>, WORLD_VOL> &voxels_world, const std::string &filename);
private:
    VoxelEngine* app;

};

#endif  // WORLD_HPP