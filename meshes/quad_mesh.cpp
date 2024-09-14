#include "quad_mesh.hpp"
#include "../voxel_engine.hpp"

// Constructor: initializes the VAO and VBO for the quad mesh
QuadMesh::QuadMesh(VoxelEngine* app) : app(app), vao(0), vbo(0) {
    // Get the shader program from the application context
    program = app->shaderProgram->waterProgram;  // Assuming you have a method to get the shader program

    // Initialize the VAO
    getVAO();
}

// Destructor: clean up OpenGL resources
QuadMesh::~QuadMesh() {
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
}

// Function to generate and bind the VAO
void QuadMesh::getVAO() {
    // Get vertex data (positions and colors)
    std::vector<float> vertexData = getVertexData();

    // Generate and bind VAO
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Generate and bind VBO
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(float), vertexData.data(), GL_STATIC_DRAW);

    // Set the attribute pointers (position and color)
    GLuint posLocation = glGetAttribLocation(program, "in_tex_coord");
    glEnableVertexAttribArray(posLocation);
    glVertexAttribPointer(posLocation, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);

    GLuint colorLocation = glGetAttribLocation(program, "in_position");
    glEnableVertexAttribArray(colorLocation);
    glVertexAttribPointer(colorLocation, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));


    // Unbind the VAO
    glBindVertexArray(0);
}

std::vector<float> QuadMesh::getVertexData() {
    // Interleave texture coordinates and vertex positions in a single step using floats
    std::vector<float> vertex_data = {
        // Tex coord (0, 0), Vertex (0, 0, 0)
        0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        // Tex coord (1, 1), Vertex (1, 0, 1)
        1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
        // Tex coord (1, 0), Vertex (1, 0, 0)
        1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        // Tex coord (0, 0), Vertex (0, 0, 0)
        0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        // Tex coord (0, 1), Vertex (0, 0, 1)
        0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        // Tex coord (1, 1), Vertex (1, 0, 1)
        1.0f, 1.0f, 1.0f, 0.0f, 1.0f
    };
    return vertex_data;
}

void QuadMesh::render() {
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}