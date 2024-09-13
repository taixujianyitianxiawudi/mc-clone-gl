#ifndef CUBE_MESH_HPP
#define CUBE_MESH_HPP


#include "../settings.hpp"

class VoxelEngine;
class CubeMesh {
public:
    CubeMesh(VoxelEngine* app);              // Constructor
    ~CubeMesh();             // Destructor

    void render();           // Function to render the cube
    std::vector<float> getVertexData();  // Get vertex data for the cube
    GLuint program;          // Shader program ID
    VoxelEngine* app;
private:
    GLuint vao, vbo;         // VAO and VBO for the cube

    void getVAO();           // Function to initialize the VAO

};

#endif // CUBE_MESH_HPP