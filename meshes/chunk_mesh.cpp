#include "chunk_mesh.hpp"
#include "../world_objects/chunk.hpp"
#include "../voxel_engine.hpp"
#include "chunk_mesh_builder.hpp"  // Assuming this will be defined to build the chunk mesh

// Constructor: initializes the chunk mesh with the chunk data
ChunkMesh::ChunkMesh(Chunk* chunk) : chunk(chunk), vao(0), vbo(0), formatSize(7) {
    program = chunk->app->shaderProgram->chunkProgram;  // Get the shader program from the app

    // Initialize the VAO and VBO
    getVAO();
}

// Destructor: clean up OpenGL resources
ChunkMesh::~ChunkMesh() {
    GLCall(glDeleteBuffers(1, &vbo));
    GLCall(glDeleteVertexArrays(1, &vao));
}

// Function to generate and bind the VAO and VBO
void ChunkMesh::getVAO() {
    // Get vertex data
    std::vector<uint8_t> vertexData = getVertexData();
    count_to_draw = vertexData.size() / formatSize;
    // Generate and bind VAO
    GLCall(glGenVertexArrays(1, &vao));
    GLCall(glBindVertexArray(vao));

    // Generate and bind VBO
    GLCall(glGenBuffers(1, &vbo));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, vbo));
    GLCall(glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(uint8_t), vertexData.data(), GL_STATIC_DRAW));

    auto strideLength = formatSize * sizeof(uint8_t);
    // Set the attribute pointers (e.g., position, voxel_id, face_id, ao_id, flip_id)
    GLuint posLocation = glGetAttribLocation(program, "in_position");
    GLCall(glVertexAttribPointer(posLocation, 3, GL_UNSIGNED_BYTE, GL_FALSE, strideLength, (void*)0));
    GLCall(glEnableVertexAttribArray(posLocation));

    GLuint voxelIdLocation = glGetAttribLocation(program, "voxel_id");
    GLCall(glVertexAttribPointer(voxelIdLocation, 1, GL_UNSIGNED_BYTE, GL_FALSE, strideLength, (void*)(3 * sizeof(uint8_t))));
    GLCall(glEnableVertexAttribArray(voxelIdLocation));

    GLuint faceIdLocation = glGetAttribLocation(program, "face_id");
    GLCall(glVertexAttribPointer(faceIdLocation, 1, GL_UNSIGNED_BYTE, GL_FALSE, strideLength, (void*)(4 * sizeof(uint8_t))));
    GLCall(glEnableVertexAttribArray(faceIdLocation));

    //GLuint aoIdLocation = glGetAttribLocation(program, "ao_id");
    //GLCall(glVertexAttribPointer(aoIdLocation, 1, GL_UNSIGNED_BYTE, GL_FALSE, strideLength, (void*)(5 * sizeof(uint8_t))));
    //GLCall(glEnableVertexAttribArray(aoIdLocation));

    //GLuint flipIdLocation = glGetAttribLocation(program, "flip_id");
    //GLCall(glVertexAttribPointer(flipIdLocation, 1, GL_UNSIGNED_BYTE, GL_FALSE, strideLength, (void*)(6 * sizeof(uint8_t))));
    //GLCall(glEnableVertexAttribArray(flipIdLocation));

    // Unbind the VAO
    GLCall(glBindVertexArray(0));
}

// Function to generate vertex data for the chunk mesh
std::vector<uint8_t> ChunkMesh::getVertexData() {
    // Use the chunk's voxel data and format size to build the chunk mesh
      // Based on 3u1 1u1 1u1 format: 3 unsigned ints for position, 1 for voxel_id, and 1 for face_id
    return buildChunkMesh(*(chunk->voxels), formatSize, chunk->position, chunk->world->voxels);
}

// Function to render the chunk mesh
void ChunkMesh::render() {
    // Use the shader program
    GLCall(glUseProgram(program));

    // Bind the VAO and draw the chunk mesh
    GLCall(glBindVertexArray(vao));
    GLCall(glDrawArrays(GL_TRIANGLES, 0, count_to_draw));  // Example: replace with actual number of vertices
    //GLCall(glDrawElements(GL_TRIANGLES, 0, count_to_draw));
    GLCall(glBindVertexArray(0));
}

