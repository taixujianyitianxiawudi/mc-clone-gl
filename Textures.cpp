#include "Textures.hpp"
#include "voxel_engine.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "assets/stb_image.h"

// Constructor to initialize textures
Textures::Textures(VoxelEngine* app) : app(app), texture_0(0) {
    // Load the texture from file
    texture_0 = load("test.png");

    // Bind the texture to texture unit 0
    GLCall(glActiveTexture(GL_TEXTURE0));
    glBindTexture(GL_TEXTURE_2D, texture_0);
}

// Destructor to clean up OpenGL resources
Textures::~Textures() {
    glDeleteTextures(1, &texture_0);
}

// Function to load a texture from file using SFML
GLuint Textures::load(const std::string& fileName) {
    // Load the texture using stb_image
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true); // Flip the image vertically

    unsigned char* data = stbi_load(("../assets/" + fileName).c_str(), &width, &height, &nrChannels, 0);
    if (!data) {
        std::cerr << "Failed to load texture: " << fileName << std::endl;
        return 0;
    }

    // Determine the format based on the number of channels
    GLenum format;
    if (nrChannels == 1)
        format = GL_RED;
    else if (nrChannels == 3)
        format = GL_RGB;
    else if (nrChannels == 4)
        format = GL_RGBA;

    // Generate an OpenGL texture
    GLuint textureID;
    glGenTextures(1, &textureID);
    GLCall(glBindTexture(GL_TEXTURE_2D, textureID));

    // Upload the texture data to OpenGL
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        format,  // Use the appropriate internal format
        width,
        height,
        0,
        format,
        GL_UNSIGNED_BYTE,
        data
    );

    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // Generate mipmaps and set anisotropic filtering
    glGenerateMipmap(GL_TEXTURE_2D);
    GLfloat maxAnisotropy;
    glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &maxAnisotropy);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY, std::min(32.0f, maxAnisotropy));

    // Unbind the texture
    GLCall(glBindTexture(GL_TEXTURE_2D, 0));

    // Free the image data
    stbi_image_free(data);

    return textureID;
}