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

    glm::vec3 velocity;
    bool on_ground;
    void applyGravity(float deltaTime);
    bool isVoxelSolid(glm::vec3 voxelPos, std::array<std::array<float, CHUNK_VOL>, WORLD_VOL>& voxels_world);

    void setFlyMode();

    void setGravityMode();

    bool fly_mode;
    void switchFlyMode();
private:
    VoxelEngine* app;

    // Control functions
    void keyboard_control(float velocity);

    void keyboard_control_walk(float velocity);

    void keyboard_block_type();

    void mouse_control();
    VoxelHandler* voxel_handler;

};



#endif //PLAYER_HPP
