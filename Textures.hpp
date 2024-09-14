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
    GLuint load(const std::string &fileName);

    GLuint loadTexArray(const std::string &fileName, int layerCount);
    GLuint grass_texture;  // OpenGL texture ID
    GLuint water_texture;
    GLuint world_texture_array;
    GLuint frame_texture;


    GLuint grass_texture_unit;
    GLuint water_texture_unit;
    GLuint world_texture_array_unit;
    GLuint frame_texture_unit;

    GLuint depth_texture;
    GLuint depth_texture_unit;

    static GLuint getDepthTexture(const glm::vec2& windowSize);

    void bindTexture(GLuint textureID, GLuint unit);

private:
    VoxelEngine* app;  // Pointer to the main application context


};

#endif // TEXTURES_HPP