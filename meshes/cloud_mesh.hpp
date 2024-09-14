#ifndef CLOUD_MESH_HPP
#define CLOUD_MESH_HPP

#include "../settings.hpp"
#include "../noise.hpp"

class VoxelEngine;

class CloudMesh {
public:
    CloudMesh(VoxelEngine* app);
    ~CloudMesh();

    void generateClouds();  // Generates cloud data
    void buildMesh();       // Builds the mesh for the clouds
    void render();          // Renders the clouds
    GLuint program;         // Shader program ID
private:
    GLuint vao, vbo;        // OpenGL VAO and VBO
    VoxelEngine* app;       // Application context


    std::vector<uint8_t> cloudData;  // Stores the cloud voxel data
    std::vector<uint16_t> meshData;  // Stores the cloud mesh data
};

#endif // CLOUD_MESH_HPP