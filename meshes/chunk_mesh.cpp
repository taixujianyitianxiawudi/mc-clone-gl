#include "chunk_mesh.hpp"
#include "chunk_mesh_builder.hpp"

#include "../voxel_engine.hpp"
#include "../world_objects/chunk.hpp"

ChunkMesh::ChunkMesh(Chunk* chunk)
    : chunk(chunk), VAO(0), VBO(0), program(0) {
    program = chunk->app->shaderProgram->chunkProgram;
    setupMesh();
}

ChunkMesh::~ChunkMesh() {
    if (VBO) glDeleteBuffers(1, &VBO);
    if (VAO) glDeleteVertexArrays(1, &VAO);
}

void ChunkMesh::setupMesh() {
    // Generate and bind VAO and VBO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(float), vertexData.data(), GL_STATIC_DRAW);

    // Vertex attribute setup (position, UV, normal, etc.)
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 7, (void*)0);  // Position

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 7, (void*)(sizeof(float) * 3));  // UVs

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(float) * 7, (void*)(sizeof(float) * 5));  // VoxelId

    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(float) * 7, (void*)(sizeof(float) * 6));  // face_id

    glBindVertexArray(0);  // Unbind VAO
}

void ChunkMesh::rebuild() {
    // Rebuild the mesh: clear the previous vertex data and generate new data
    vertexData.clear();

    // Use ChunkMeshBuilder to rebuild the mesh data
    ChunkMeshBuilder::buildMesh(chunk, vertexData);

    // Re-upload the new vertex data to the GPU
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(float), vertexData.data(), GL_STATIC_DRAW);
}

void ChunkMesh::render() const {
    if (!vertexData.empty()) {
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, vertexData.size() / 7);  // Assuming 5 floats per vertex (3 position, 2 UV)
        glBindVertexArray(0);
    }
}

GLuint ChunkMesh::getProgram() const {
    return program;
}
