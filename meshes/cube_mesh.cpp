#include "cube_mesh.hpp"
#include <iostream>
#include "../voxel_engine.hpp"

// Constructor: initializes the VAO and VBO for the cube mesh
CubeMesh::CubeMesh(VoxelEngine* app) : vao(0), vbo(0), app(app) {
    // Load your shader program (assuming you have a way to load it)
    // For example: program = loadShader("voxel_shader.vert", "voxel_shader.frag");
    program = app->shaderProgram->voxel_marker_program;
        // Load your shader program here;

    // Initialize the VAO
    getVAO();
}

// Destructor: clean up OpenGL resources
CubeMesh::~CubeMesh() {
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
}

// Function to generate and bind the VAO
void CubeMesh::getVAO() {
    // Get vertex data (positions and texture coordinates)
    std::vector<float> vertexData = getVertexData();

    // Generate and bind VAO
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Generate and bind VBO
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(float), vertexData.data(), GL_STATIC_DRAW);

    // Set the attribute pointers for position and texture coordinates
    GLuint posLocation = glGetAttribLocation(program, "in_position");
    glVertexAttribPointer(posLocation, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(posLocation);

    GLuint texCoordLocation = glGetAttribLocation(program, "in_tex_coord_0");
    glVertexAttribPointer(texCoordLocation, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(texCoordLocation);

    // Unbind the VAO
    glBindVertexArray(0);
}

// Function to return the vertex data for the cube (positions and texture coordinates interleaved)
std::vector<float> CubeMesh::getVertexData() {
    // Define vertices for the cube (positions)
    std::vector<glm::vec3> vertices = {
        glm::vec3(0.0f, 0.0f, 1.0f),  // Front-bottom-left
        glm::vec3(1.0f, 0.0f, 1.0f),  // Front-bottom-right
        glm::vec3(1.0f, 1.0f, 1.0f),  // Front-top-right
        glm::vec3(0.0f, 1.0f, 1.0f),  // Front-top-left
        glm::vec3(0.0f, 1.0f, 0.0f),  // Back-top-left
        glm::vec3(0.0f, 0.0f, 0.0f),  // Back-bottom-left
        glm::vec3(1.0f, 0.0f, 0.0f),  // Back-bottom-right
        glm::vec3(1.0f, 1.0f, 0.0f)   // Back-top-right
    };

    // Indices for each face (two triangles per face)
    std::vector<glm::ivec3> indices = {
        glm::ivec3(0, 1, 2), glm::ivec3(0, 2, 3),  // Front face
        glm::ivec3(1, 6, 7), glm::ivec3(1, 7, 2),  // Right face
        glm::ivec3(6, 5, 4), glm::ivec3(6, 4, 7),  // Back face
        glm::ivec3(5, 0, 3), glm::ivec3(5, 3, 4),  // Left face
        glm::ivec3(3, 2, 7), glm::ivec3(3, 7, 4),  // Top face
        glm::ivec3(5, 6, 1), glm::ivec3(5, 1, 0)   // Bottom face
    };

    // Fixed UV coordinates for each face
    std::vector<glm::vec2> texCoords = {
        { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f }  // Standard UVs for faces
    };

    // Create vertex data by interleaving positions and texture coordinates
    std::vector<float> vertexData;
    for (const auto& triangle : indices) {
        for (int i = 0; i < 3; ++i) {
            glm::vec3 vertex = vertices[triangle[i]];
            // Push position data
            vertexData.push_back(vertex.x);
            vertexData.push_back(vertex.y);
            vertexData.push_back(vertex.z);

            // Push corresponding texture coordinate (ensure correct alignment with face orientation)
            glm::vec2 uv = texCoords[i % 4];  // Use UV mapping per face
            vertexData.push_back(uv.x);
            vertexData.push_back(uv.y);
        }
    }

    return vertexData;
}

void CubeMesh::render() {
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 36);  // 36 vertices (6 faces, 2 triangles per face)
    glBindVertexArray(0);
}
