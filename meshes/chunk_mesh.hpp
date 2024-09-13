#ifndef CHUNK_MESH_HPP
#define CHUNK_MESH_HPP

#include "../settings.hpp"  // Assuming you have settings like CHUNK_SIZE, CHUNK_AREA


class Chunk;
class ChunkMeshBuilder;
class VoxelEngine;

class ChunkMesh {
public:
    // Constructor: takes a pointer to the owning chunk
    ChunkMesh(Chunk* chunk);

    // Destructor
    ~ChunkMesh();

    // Rebuild the mesh (clears the previous data)
    void rebuild();

    // Render the mesh
    void render() const;

    // Get OpenGL program ID (for shader use)
    GLuint getProgram() const;
    GLuint program;  // Shader program ID

private:
    Chunk* chunk;  // Reference to the owning chunk
    GLuint VAO, VBO;  // OpenGL handles for the vertex array and buffer objects


    std::vector<float> vertexData;  // Vertex data

    // Internal function to create the VAO and VBO
    void setupMesh();

    // Friend class for mesh building
    friend class ChunkMeshBuilder;
};

#endif // CHUNK_MESH_HPP
