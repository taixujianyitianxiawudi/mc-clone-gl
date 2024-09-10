#ifndef TEXTURES_HPP
#define TEXTURES_HPP

#include "settings.hpp"

class VoxelEngine;  // Forward declaration of the VoxelEngine class

class Textures {
public:
    // Constructor to initialize textures
    Textures(VoxelEngine* app);
    // Destructor to clean up OpenGL resources
    ~Textures();

    // Load a texture from a file
    GLuint load(const std::string& fileName);

private:
    VoxelEngine* app;  // Pointer to the main application context

    GLuint texture_0;  // OpenGL texture ID
};

#endif // TEXTURES_HPP
