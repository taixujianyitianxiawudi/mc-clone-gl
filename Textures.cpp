#include "Textures.hpp"
#include "voxel_engine.hpp"


// Constructor to initialize textures
Textures::Textures(VoxelEngine* app) : app(app), texture_0(0) {
    // Load the texture from file
    texture_0 = load("grass.png");

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
    // Load the texture using SFML
    sf::Image image;
    if (!image.loadFromFile("../assets/" + fileName)) {
        std::cerr << "Failed to load texture: " << fileName << std::endl;
        return 0;
    }

    // Flip the image on the y-axis (optional)
    image.flipVertically();

    // Generate an OpenGL texture
    GLuint textureID;
    glGenTextures(1, &textureID);
    GLCall(glBindTexture(GL_TEXTURE_2D, textureID));

    // Upload the texture data to OpenGL
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGBA,
        image.getSize().x,
        image.getSize().y,
        0,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        image.getPixelsPtr()
    );

    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // Generate mipmaps and set anisotropic filtering
    glGenerateMipmap(GL_TEXTURE_2D);
    GLfloat maxAnisotropy;
    glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &maxAnisotropy);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY, std::min(32.0f, maxAnisotropy));

    // Unbind the texture
    GLCall(glBindTexture(GL_TEXTURE_2D, 0));

    return textureID;
}