#include "Textures.hpp"
#include "voxel_engine.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "assets/stb_image.h"

// Constructor to initialize textures
Textures::Textures(VoxelEngine* app) : app(app) {
    // Load the texture from file
    grass_texture = load("grass.png");
    water_texture = load("water.png");
    world_texture_array = loadTexArray("tex_array_0.png", 8);
    frame_texture = load("frame.png");

    depth_texture = getDepthTexture(WIN_RES);

    // Bind the texture to texture unit 0
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, grass_texture);
    grass_texture_unit = 0;

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D_ARRAY, world_texture_array);
    world_texture_array_unit = 1;

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, water_texture);
    water_texture_unit = 2;

    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, frame_texture);
    frame_texture_unit = 3;

    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, depth_texture);
    depth_texture_unit = 4;


}

// Destructor to clean up OpenGL resources
Textures::~Textures() {
    glDeleteTextures(1, &grass_texture);
    glDeleteTextures(1, &water_texture);
    glDeleteTextures(1, &world_texture_array);
    glDeleteTextures(1, &frame_texture);
}


// Function to load a texture from file using SFML
GLuint Textures::load(const std::string& fileName) {
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);

    std::string fullPath = "../assets/" + fileName;
    unsigned char* data = stbi_load(fullPath.c_str(), &width, &height, &nrChannels, 4);  // Load as RGBA
    if (!data) {
        std::cerr << "Failed to load texture: " << fileName << std::endl;
        return 0;
    }

    GLuint textureID;
    glGenTextures(1, &textureID);

    // Regular 2D texture
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);


    GLfloat maxAnisotropy;
    glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &maxAnisotropy);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY, std::min(32.0f, maxAnisotropy));

    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(data);

    return textureID;
}

// Function to load a texture array from file
GLuint Textures::loadTexArray(const std::string& fileName, int layerCount) {
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(false);


    std::string fullPath = "../assets/" + fileName;
    unsigned char* data = stbi_load(fullPath.c_str(), &width, &height, &nrChannels, 4);  // Load as RGBA
    if (!data) {
        std::cerr << "Failed to load texture: " << fileName << std::endl;
        return 0;
    }

    GLuint textureID;
    glGenTextures(1, &textureID);

    // Bind texture as a 2D array texture
    glBindTexture(GL_TEXTURE_2D_ARRAY, textureID);

    // Specify the 3D texture (2D array)
    glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGBA, width, height / layerCount, layerCount, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D_ARRAY);

    GLfloat maxAnisotropy;
    glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &maxAnisotropy);
    glTexParameterf(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAX_ANISOTROPY, std::min(32.0f, maxAnisotropy));

    glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
    stbi_image_free(data);

    return textureID;
}

GLuint Textures::getDepthTexture(const glm::vec2& windowSize) {
    GLuint depthTexture;
    glGenTextures(1, &depthTexture);
    glBindTexture(GL_TEXTURE_2D, depthTexture);

    // Allocate depth texture storage
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, windowSize.x, windowSize.y, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

    // Set texture parameters for depth texture
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);  // No repeating
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);  // No repeating
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);    // No filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glBindTexture(GL_TEXTURE_2D, 0);  // Unbind the texture

    return depthTexture;
}
