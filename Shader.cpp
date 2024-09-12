#include "Shader.hpp"
#include "voxel_engine.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

// Constructor
ShaderProgram::ShaderProgram(VoxelEngine* app)
    : app(app) {
    // Load the quad shader program
    //quadProgram = getProgram("quad");
    chunkProgram = getProgram("chunk1");
    // Set the initial uniforms
    setUniformsOnInit();
}

// Destructor
ShaderProgram::~ShaderProgram() {
    //glDeleteProgram(quadProgram);  // Delete the shader program when done
    glDeleteProgram(chunkProgram);
}

// Set initial uniforms
void ShaderProgram::setUniformsOnInit() {
    // Assuming that the player and context are already set up in the application (VoxelEngine)
    glm::mat4 proj = app->player->get_projection_matrix();
    glm::mat4 model = glm::mat4(1.0f);  // Identity matrix for the model

    //use(quadProgram);  // Activate the shader program
    //setMatrixUniform(quadProgram, "m_proj", proj);
    //setMatrixUniform(quadProgram, "m_model", model);

    use(chunkProgram);
    setMatrixUniform(chunkProgram, "m_proj", proj);
    setMatrixUniform(chunkProgram, "m_model", model);
    setTextureUniform(chunkProgram, "u_texture_0", 0);
}

// Update the uniforms (for example, the view matrix)
void ShaderProgram::update() {
    glm::mat4 view = app->player->get_view_matrix();  // Get the view matrix from the player
    //use(quadProgram);  // Activate the shader program
    //setMatrixUniform(quadProgram, "m_view", view);  // Update the view matrix uniform
    use(chunkProgram);
    setMatrixUniform(chunkProgram, "m_view", view);
}

// Use the shader program
void ShaderProgram::use(GLuint program) {
    glUseProgram(program);
}

// Helper function to load shaders from file and compile them
GLuint ShaderProgram::loadShader(const std::string& filePath, GLenum shaderType) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open shader file " << filePath << std::endl;
        return 0;
    }

    std::stringstream shaderStream;
    shaderStream << file.rdbuf();
    std::string shaderCode = shaderStream.str();
    const char* shaderSource = shaderCode.c_str();

    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &shaderSource, nullptr);
    glCompileShader(shader);

    // Check for compilation errors
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cerr << "Error: Shader compilation failed\n" << infoLog << std::endl;
        return 0;
    }

    return shader;
}

// Load and compile a shader program (vertex and fragment shaders)
GLuint ShaderProgram::getProgram(const std::string& shaderName) {
    std::string vertexPath = "../shaders/" + shaderName + ".vert";
    std::string fragmentPath = "../shaders/" + shaderName + ".frag";

    GLuint vertexShader = loadShader(vertexPath, GL_VERTEX_SHADER);
    GLuint fragmentShader = loadShader(fragmentPath, GL_FRAGMENT_SHADER);

    // Create and link the shader program
    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    // Check for linking errors
    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(program, 512, nullptr, infoLog);
        std::cerr << "Error: Shader program linking failed\n" << infoLog << std::endl;
        return 0;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return program;
}

// Set a matrix uniform
void ShaderProgram::setMatrixUniform(GLuint program, const std::string& name, const glm::mat4& matrix) {
    GLuint location = glGetUniformLocation(program, name.c_str());
    if (location == -1) {
        std::cerr << "Error: Uniform " << name << " not found in shader program " << program << std::endl;
        return;
    }
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}

void ShaderProgram::setTextureUniform(GLuint program, const std::string &name, GLuint texture) {
    GLuint location = glGetUniformLocation(program, name.c_str());
    if (location == -1) {
        std::cerr << "Error: Uniform " << name << " not found in shader program " << program << std::endl;
        return;
    }
    glUniform1i(location, texture);
}
