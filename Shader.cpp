#include "Shader.hpp"
#include "voxel_engine.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <variant>

// Constructor
ShaderProgram::ShaderProgram(VoxelEngine* app)
    : app(app) {
    // Load the quad shader program
    waterProgram = getProgram("water");
    chunkProgram = getProgram("chunk");
    //chunkProgram = getProgram("chunk_shadow");
    voxel_marker_program = getProgram("voxel_marker");
    cloudsProgram = getProgram("clouds");
    shadowProgram = getProgram("shadow_map");
    // Set the initial uniforms
    setUniformsOnInit();
}

// Destructor
ShaderProgram::~ShaderProgram() {
    glDeleteProgram(waterProgram);  // Delete the shader program when done
    glDeleteProgram(chunkProgram);
    glDeleteProgram(voxel_marker_program);
    glDeleteProgram(cloudsProgram);
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
    setFloatUniform(chunkProgram,"water_line", WATER_LINE);
    setVec3Uniform(chunkProgram, "bg_color", BG_COLOR);
    //setTextureUniform(chunkProgram, "texture_diffuse", 0);
    GLuint texture_array_0 = app->textures->world_texture_array;
    GLuint texture_array_unit = app->textures->world_texture_array_unit;
    setTextureArrayUniform(chunkProgram, "u_texture_array_0", texture_array_0, texture_array_unit);


    use(voxel_marker_program);
    setMatrixUniform(voxel_marker_program, "m_proj", proj);
    setMatrixUniform(voxel_marker_program, "m_model", model);
    GLuint texture = app->textures->frame_texture;
    GLuint texture_unit = app->textures->frame_texture_unit;
    setTextureUniform(voxel_marker_program, "u_texture_0", texture, texture_unit);

    use(waterProgram);
    setMatrixUniform(waterProgram, "m_proj", proj);
    GLuint texture_water = app->textures->water_texture_unit;
    GLuint texture_unit_water = app->textures->water_texture_unit;
    setTextureUniform(waterProgram, "u_texture_0", texture_water, texture_unit_water);
    setIntUniform(waterProgram, "water_area", WATER_AREA);
    setFloatUniform(waterProgram, "water_line", WATER_LINE);

    use(shadowProgram);
    setMatrixUniform(shadowProgram, "m_proj", proj);
    setMatrixUniform(shadowProgram, "m_model", model);
}

// Update the uniforms (for example, the view matrix)
void ShaderProgram::update() {
    glm::mat4 view = app->player->get_view_matrix();  // Get the view matrix from the player
    //use(quadProgram);  // Activate the shader program
    //setMatrixUniform(quadProgram, "m_view", view);  // Update the view matrix uniform
    use(chunkProgram);
    setMatrixUniform(chunkProgram, "m_view", view);
    use(voxel_marker_program);
    setMatrixUniform(voxel_marker_program, "m_view", view);
    use(waterProgram);
    setMatrixUniform(waterProgram, "m_view", view);
    //use(shadowProgram);
    //setMatrixUniform(shadowProgram, "m_view_light", view);
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

void ShaderProgram::setupShadowMapping(int width, int height) {
    shadowFramebuffer = new ShadowFramebuffer(width, height);
}

// Set a matrix uniform
void ShaderProgram::setMatrixUniform(GLuint program, const std::string& name, const glm::mat4& matrix) {
    glUseProgram(program);
    GLuint location = glGetUniformLocation(program, name.c_str());
    if (location == -1) {
        std::cerr << "Error: Uniform " << name << " not found in shader program " << program << std::endl;
        return;
    }
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}

void ShaderProgram::setUnsignedIntUniform(GLuint program, const std::string &name, int value) {
    glUseProgram(program);
    GLuint location = glGetUniformLocation(program, name.c_str());
    if (location == -1) {
        std::cerr << "Error: Uniform " << name << " not found in shader program " << program << std::endl;
        return;
    }
    glUniform1ui(location, value);
}

void ShaderProgram::setIntUniform(GLuint program, const std::string &name, int value) {
    glUseProgram(program);
    GLuint location = glGetUniformLocation(program, name.c_str());
    if (location == -1) {
        std::cerr << "Error: Uniform " << name << " not found in shader program " << program << std::endl;
        return;
    }
    glUniform1i(location, value);
}

void ShaderProgram::setFloatUniform(GLuint program, const std::string &name, float value) {
    glUseProgram(program);
    GLuint location = glGetUniformLocation(program, name.c_str());
    if (location == -1) {
        std::cerr << "Error: Uniform " << name << " not found in shader program " << program << std::endl;
        return;
    }
    glUniform1f(location, value);
}

void ShaderProgram::setVec3Uniform(GLuint program, const std::string &name, const glm::vec3 &value) {
    glUseProgram(program);
    GLuint location = glGetUniformLocation(program, name.c_str());
    if (location == -1) {
        std::cerr << "Error: Uniform " << name << " not found in shader program " << program << std::endl;
        return;
    }
    glUniform3fv(location, 1, glm::value_ptr(value));
}

void ShaderProgram::setTextureArrayUniform(GLuint program, const std::string& name, GLuint textureArray, GLuint textureUnit) {
    // Use the shader program
    glUseProgram(program);

    // Get the location of the uniform variable in the shader
    GLint location = glGetUniformLocation(program, name.c_str());
    if (location == -1) {
        std::cerr << "Warning: uniform '" << name << "' not found in shader program!" << std::endl;
        return;
    }

    // Activate the appropriate texture unit (e.g., GL_TEXTURE0 + textureUnit)
    glActiveTexture(GL_TEXTURE0 + textureUnit);

    // Bind the texture array to the active texture unit
    glBindTexture(GL_TEXTURE_2D_ARRAY, textureArray);

    // Set the uniform to point to the active texture unit
    glUniform1i(location, textureUnit);
}


void ShaderProgram::setTextureUniform(GLuint program, const std::string &name, GLuint texture, GLuint textureUnit) {
    glUseProgram(program);

    // Get the location of the texture uniform in the shader
    GLuint location = glGetUniformLocation(program, name.c_str());
    if (location == -1) {
        std::cerr << "Error: Uniform " << name << " not found in shader program " << program << std::endl;
        return;
    }

    // Activate the appropriate texture unit (e.g., GL_TEXTURE0 + textureUnit)
    glActiveTexture(GL_TEXTURE0 + textureUnit);

    // Bind the texture to the active texture unit
    glBindTexture(GL_TEXTURE_2D, texture);

    // Set the uniform to the texture unit index
    glUniform1i(location, textureUnit);
}


// Render the shadow map to the depth texture
void ShaderProgram::renderShadowMap() {
    // First pass: Render scene from the light's perspective into the shadow framebuffer
    shadowFramebuffer->bind();
    glViewport(0, 0, WIN_RES.x, WIN_RES.y);
    glClear(GL_DEPTH_BUFFER_BIT);

    glUseProgram(shadowProgram);

    glm::mat4 lightSpaceMatrix = app->scene->light->getLightSpaceMatrix();  // Assuming you have a light class
    setMatrixUniform(shadowProgram, "lightSpaceMatrix", lightSpaceMatrix);

    // Render the scene (just depth)
    //renderScene(shadowProgram);
    app->scene->world->render_with_shader_program(shadowProgram);

    shadowFramebuffer->unbind();
}

// Bind the shadow map to the final rendering pass
void ShaderProgram::bindShadowMap() {
    glUseProgram(chunkProgram);

    // Bind the shadow map texture from the framebuffer
    GLuint shadowMapTexture = shadowFramebuffer->getDepthTexture();

    // Set shadow map as uniform
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, shadowMapTexture);
    setTextureUniform(chunkProgram, "shadowMap", shadowMapTexture, 1);

    // Set the light space matrix uniform
    glm::mat4 lightSpaceMatrix = app->scene->light->getLightSpaceMatrix();
    setMatrixUniform(chunkProgram, "lightSpaceMatrix", lightSpaceMatrix);
}