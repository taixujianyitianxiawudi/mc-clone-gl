#ifndef SHADERPROGRAM_HPP
#define SHADERPROGRAM_HPP

#include "settings.hpp"
#include "ShadowFramebuffer.hpp"

class VoxelEngine;  // Forward declaration of VoxelEngine

class ShaderProgram {
public:
    ShaderProgram(VoxelEngine* app);  // Constructor
    ~ShaderProgram();  // Destructor

    void use(GLuint program);  // Use the shader program
    void setUniformsOnInit();  // Set initial uniforms
    void update();  // Update the uniforms during runtime
    GLuint waterProgram;  // The OpenGL shader program ID
    GLuint chunkProgram;
    GLuint voxel_marker_program;
    GLuint cloudsProgram;

    VoxelEngine* app;  // Reference to the main application (VoxelEngine)
    GLuint shadowProgram;

    GLuint loadShader(const std::string& filePath, GLenum shaderType);  // Load and compile a shader
    GLuint getProgram(const std::string& shaderName);  // Load the shader program from file

    ShadowFramebuffer* shadowFramebuffer;
    void setupShadowMapping(int width, int height);
    void renderShadowMap();                          // Render to shadow map (depth only)
    void bindShadowMap();

    void setMatrixUniform(GLuint program, const std::string& name, const glm::mat4& matrix);  // Helper to set uniform matrices

    void setUnsignedIntUniform(GLuint program, const std::string& name, int value);  // Helper to set uniform integers
    void setIntUniform(GLuint program, const std::string& name, int value);  // Helper to set uniform integers
    void setFloatUniform(GLuint program, const std::string &name, float value);
    void setVec3Uniform(GLuint program, const std::string &name, const glm::vec3 &value);

    void setTextureUniform(GLuint program, const std::string &name, GLuint texture, GLuint textureUnit);
    void setTextureArrayUniform(GLuint program, const std::string &name, GLuint textureArray, GLuint textureUnit);
};

#endif // SHADERPROGRAM_HPP