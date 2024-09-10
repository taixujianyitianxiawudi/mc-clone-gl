#include "Player.hpp"

#include "voxel_engine.hpp"

// Constructor for Player class
Player::Player(VoxelEngine* app, const glm::vec3& position, float yaw, float pitch)
    : Camera(position, yaw, pitch), app(app) {}

// Update function to handle mouse and keyboard input and camera updates
void Player::update(float deltaTime) {
    float velocity = PLAYER_SPEED * deltaTime;
    keyboard_control(velocity);
    mouse_control();
    Camera::update();  // Call the update method from the Camera class
}

// Handle mouse input for controlling the yaw and pitch
void Player::mouse_control() {
    sf::Vector2i mouseDelta = sf::Mouse::getPosition(*app->window) - sf::Vector2i(app->window->getSize().x / 2, app->window->getSize().y / 2);
    sf::Mouse::setPosition(sf::Vector2i(app->window->getSize().x / 2, app->window->getSize().y / 2), *app->window);  // Reset mouse to center

    if (mouseDelta.x != 0) {
        rotate_yaw(mouseDelta.x * MOUSE_SENSITIVITY);
    }
    if (mouseDelta.y != 0) {
        rotate_pitch(mouseDelta.y * MOUSE_SENSITIVITY);
    }
}

// Handle keyboard input for moving the player in the world
void Player::keyboard_control(float velocity) {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        move_forward(velocity);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        move_back(velocity);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        move_right(velocity);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        move_left(velocity);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
        move_up(velocity);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {
        move_down(velocity);
    }
}
