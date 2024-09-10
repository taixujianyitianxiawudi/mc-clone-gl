#include "quad_mesh.hpp"
#include "../voxel_engine.hpp"

// Constructor: initializes the VAO and VBO for the quad mesh
QuadMesh::QuadMesh(VoxelEngine* app) : app(app), vao(0), vbo(0) {
    // Get the shader program from the application context
    program = app->shaderProgram->quadProgram;  // Assuming you have a method to get the shader program

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
    GLuint posLocation = glGetAttribLocation(program, "in_position");
    glVertexAttribPointer(posLocation, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(posLocation);

    GLuint colorLocation = glGetAttribLocation(program, "in_color");
    glVertexAttribPointer(colorLocation, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(colorLocation);

    // Unbind the VAO
    glBindVertexArray(0);
}

// Function to return the vertex data for the quad (positions and colors interleaved)
std::vector<float> QuadMesh::getVertexData() {
    // Vertices for two triangles forming a quad
    std::vector<float> vertexData = {
        // Position       // Color
         0.5f,  0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  // Top-right, green
        -0.5f,  0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  // Top-left, red
        -0.5f, -0.5f, 0.0f,  1.0f, 1.0f, 0.0f,  // Bottom-left, yellow

         0.5f,  0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  // Top-right, green
        -0.5f, -0.5f, 0.0f,  1.0f, 1.0f, 0.0f,  // Bottom-left, yellow
         0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f   // Bottom-right, blue
    };
    std::cout << vertexData.size();
    return vertexData;
}

// Function to render the quad mesh
void QuadMesh::render() {
    // Use the shader program
    glUseProgram(program);

    // Bind the VAO and render the quad
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);  // 6 vertices (2 triangles)
    glBindVertexArray(0);
}