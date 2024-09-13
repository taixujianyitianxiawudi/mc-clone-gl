//
// Created by zzk on 2024/9/5.
//

#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "settings.hpp"
#include "Camera.hpp"
#include "voxel_handler.hpp"


class VoxelEngine;

class Player : public Camera {
public:
    Player(VoxelEngine* app, const glm::vec3& position = PLAYER_POS, float yaw = -45.0f, float pitch = 0.0f);

    // Update player state (keyboard and mouse)
    void update(float deltaTime);
    void handle_event(sf::Event event);

private:
    VoxelEngine* app;

    // Control functions
    void keyboard_control(float velocity);
    void mouse_control();
    VoxelHandler* voxel_handler;
};



#endif //PLAYER_HPP
