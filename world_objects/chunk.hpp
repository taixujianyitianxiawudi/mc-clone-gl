#ifndef CHUNK_HPP
#define CHUNK_HPP

#include <memory>

#include "../settings.hpp"

  // For simplex noise
#include "../meshes/chunk_mesh.hpp"
class World;
class VoxelEngine;

class Chunk {
public:
    // Constructor: takes a pointer to the App object
    Chunk(World* world, glm::ivec3 position);

    // Build the chunk mesh
    void buildMesh();

    // Get model matrix
    glm::mat4 getModelMatrix() const;

    // Set uniforms for rendering
    void setUniform();

    // Render the chunk mesh
    void render();

    // Build voxel data

    std::array<float, CHUNK_VOL> buildVoxels();
    std::array<float, CHUNK_VOL> buildVoxels_new();
    // Set the voxel data
    void setVoxels(float* voxelData);

    VoxelEngine* app;  // Pointer to the application context
    World* world;
    //std::array<int, CHUNK_VOL> voxels;  // Voxel data
    std::array<float, CHUNK_VOL>* voxels_chunk;
    ChunkMesh* mesh;  // Mesh representation of the chunk
    glm::ivec3 position;
    glm::mat4 m_model;
    bool is_empty;
    glm::vec3 center;

    void generate_terrain(std::array<float, CHUNK_VOL>& voxels, int cx, int cy, int cz);
    bool is_on_frustum;

};

#endif // CHUNK_HPP