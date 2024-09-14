#ifndef VOXEL_ENGINE_HPP
#define VOXEL_ENGINE_HPP

#include <memory>

#include "Scene.hpp"
#include "settings.hpp"
#include "Shader.hpp"
#include "Player.hpp"
#include "Textures.hpp"

class VoxelEngine {
public:
    VoxelEngine();
    ~VoxelEngine();
    void run();
    // Context (for OpenGL)
    // Shader program, scene, and player references
    sf::Window* window;
    std::unique_ptr<Player> player;
    std::unique_ptr<ShaderProgram> shaderProgram;
    std::unique_ptr<Scene> scene;
    std::unique_ptr<Textures> textures;
    float time;

private:
    void update();
    void render();
    void handleEvents();
    void on_init();

    sf::Clock clock;
    float deltaTime;

    bool isRunning;

};

#endif // VOXEL_ENGINE_HPP