#ifndef CHUNK_HPP
#define CHUNK_HPP

#include <memory>

#include "../settings.hpp"

  // For simplex noise
#include "../meshes/chunk_mesh.hpp"
class World;

class Chunk {
public:
    // Constructor: takes a pointer to the App object
    Chunk(World* world, glm::vec3 position);

    // Build the chunk mesh
    void buildMesh();

    // Get model matrix
    glm::mat4 getModelMatrix() const;

    // Set uniforms for rendering
    void setUniform();

    // Render the chunk mesh
    void render();

    // Build voxel data

    std::array<int, CHUNK_VOL> buildVoxels();
    // Set the voxel data
    void setVoxels(int* voxelData);

    VoxelEngine* app;  // Pointer to the application context
    World* world;
    //std::array<int, CHUNK_VOL> voxels;  // Voxel data
    std::array<int, CHUNK_VOL>* voxels;
    ChunkMesh* mesh;  // Mesh representation of the chunk
    glm::vec3 position;
    glm::mat4 m_model;
    bool is_empty;


};

#endif // CHUNK_HPP