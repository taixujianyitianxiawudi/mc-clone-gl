#ifndef QUADMESH_HPP
#define QUADMESH_HPP

#include "../settings.hpp"

class VoxelEngine;  // Forward declaration of VoxelEngine (Application context)

class QuadMesh {
public:
    QuadMesh(VoxelEngine* app);  // Constructor
    ~QuadMesh();                 // Destructor

    void render();  // Function to render the quad mesh

private:
    GLuint vao, vbo;  // Vertex Array Object and Vertex Buffer Object
    GLuint program;   // Shader program ID
    VoxelEngine* app; // Pointer to the main application (for accessing context and shader)

    std::vector<float> getVertexData();  // Function to get vertex data
    void getVAO();  // Function to initialize the VAO
};

#endif // QUADMESH_HPP