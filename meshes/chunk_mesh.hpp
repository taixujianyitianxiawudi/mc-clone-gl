#ifndef CHUNKMESH_HPP
#define CHUNKMESH_HPP

#include "../settings.hpp"


class Chunk;
class VoxelEngine;

class ChunkMesh {
public:
    ChunkMesh(Chunk* chunk);  // Constructor
    ~ChunkMesh();             // Destructor

    void render();  // Render the chunk mesh
    GLuint program;       // Shader program ID
private:
    GLuint vao, vbo;      // OpenGL buffers

    Chunk* chunk;         // Pointer to the chunk
    int count_to_draw;
    std::vector<uint8_t> getVertexData();  // Function to generate vertex data
    void getVAO();  // Function to initialize VAO and VBO
    int formatSize;
};

#endif // CHUNKMESH_HPP
