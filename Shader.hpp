#ifndef SHADERPROGRAM_HPP
#define SHADERPROGRAM_HPP

#include "settings.hpp"

class VoxelEngine;  // Forward declaration of VoxelEngine

class ShaderProgram {
public:
    ShaderProgram(VoxelEngine* app);  // Constructor
    ~ShaderProgram();  // Destructor

    void use(GLuint program);  // Use the shader program
    void setUniformsOnInit();  // Set initial uniforms
    void update();  // Update the uniforms during runtime
    GLuint quadProgram;  // The OpenGL shader program ID
    GLuint chunkProgram;
    GLuint voxel_marker_program;

    VoxelEngine* app;  // Reference to the main application (VoxelEngine)
    GLuint cloudsProgram;

    GLuint loadShader(const std::string& filePath, GLenum shaderType);  // Load and compile a shader
    GLuint getProgram(const std::string& shaderName);  // Load the shader program from file
    void setMatrixUniform(GLuint program, const std::string& name, const glm::mat4& matrix);  // Helper to set uniform matrices
    void setTextureUniform(GLuint program, const std::string& name, GLuint texture);  // Helper to set uniform textures
    void setUnsignedIntUniform(GLuint program, const std::string& name, int value);  // Helper to set uniform integers
    void setIntUniform(GLuint program, const std::string& name, int value);  // Helper to set uniform integers
};

#endif // SHADERPROGRAM_HPP
