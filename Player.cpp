#include "Player.hpp"

#include "voxel_engine.hpp"

// Constructor for Player class
Player::Player(VoxelEngine* app, const glm::vec3& position, float yaw, float pitch)
    : Camera(position, yaw, pitch), app(app), fly_mode(true) {
    //voxel_handler = app->scene->world.voxel_handler;
}

// Update function to handle mouse and keyboard input and camera updates
void Player::update(float deltaTime) {
    keyboard_block_type();
    float velocity = PLAYER_SPEED * deltaTime;
    if (!fly_mode) {
        velocity = velocity / 2;
        this->velocity.x = 0;
        this->velocity.z = 0;
        mouse_control();
        keyboard_control_walk(velocity);
        float calculated_velocity_x = this->velocity.x * forward.x + this->velocity.z * right.x;
        float calculated_velocity_z = this->velocity.x * forward.z + this->velocity.z * right.z;
        // Normalize the velocity

        // Normalize the velocity
        float magnitude = sqrt(calculated_velocity_x * calculated_velocity_x + calculated_velocity_z * calculated_velocity_z);

        if (magnitude > 0.0f) {
            calculated_velocity_x /= magnitude;
            calculated_velocity_z /= magnitude;
        }
        calculated_velocity_x *= deltaTime * 3;
        calculated_velocity_z *= deltaTime * 3;

        this->velocity.x = calculated_velocity_x;
        this->velocity.z = calculated_velocity_z;
        applyGravity(deltaTime);
        position.x += this->velocity.x ;
        position.z += this->velocity.z ;
        //move_forward(this->velocity.x);
        //move_up(this->velocity.y * deltaTime);
        //move_right(this->velocity.z);
    }
    if (fly_mode) {
        keyboard_control(velocity);
        mouse_control();
    }
    Camera::update();  // Call the update method from the Camera class
}

void Player::handle_event(sf::Event event) {
    // Handle mouse button clicks for adding and removing voxels
    if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Left) {
            // Left mouse button pressed - set voxel
            app->scene->world->voxel_handler->setVoxel();
        } else if (event.mouseButton.button == sf::Mouse::Right) {
            // Right mouse button pressed - switch mode (add/remove)
            app->scene->world->voxel_handler->switchMode();
        }
    }
}

void Player::applyGravity(float deltaTime) {
    if (!on_ground) {
        // Apply gravity to the player's vertical velocity
        velocity.y += GRAVITY * deltaTime;

        // Cap the fall speed to avoid excessive falling speed
        if (velocity.y < MAX_FALL_SPEED) {
            velocity.y = MAX_FALL_SPEED;
        }

        // Move the player down by the velocity
        move_up(velocity.y * deltaTime);

    }
    // Check if the player has hit the ground (collision check)

    float segment = std::max(deltaTime * PLAYER_SPEED , 0.2f);
    glm::vec3 playerPos = get_position();
    glm::vec3 belowPlayer = glm::vec3(playerPos.x + 0.1f, playerPos.y - 1.6f, playerPos.z - 0.1f);
    glm::vec3 belowPlayer1 = glm::vec3(playerPos.x - 0.1f, playerPos.y - 1.6f, playerPos.z - 0.1f);
    glm::vec3 belowPlayer2 = glm::vec3(playerPos.x  - 0.1f, playerPos.y - 1.6f, playerPos.z + 0.1f);
    glm::vec3 belowPlayer3 = glm::vec3(playerPos.x + 0.1f, playerPos.y - 1.6f, playerPos.z + 0.1f);

    glm::vec3 frontPlayer = glm::vec3(playerPos.x, playerPos.y, playerPos.z - segment);
    glm::vec3 frontPlayer_low = glm::vec3(playerPos.x, playerPos.y - 1.59f, playerPos.z - segment);
    glm::vec3 backPlayer = glm::vec3(playerPos.x, playerPos.y, playerPos.z + segment);
    glm::vec3 backPlayer_low = glm::vec3(playerPos.x, playerPos.y - 1.59f, playerPos.z + segment);
    glm::vec3 rightPlayer = glm::vec3(playerPos.x - segment, playerPos.y, playerPos.z);
    glm::vec3 rightPlayer_low = glm::vec3(playerPos.x - segment, playerPos.y - 1.59f, playerPos.z);
    glm::vec3 leftPlayer = glm::vec3(playerPos.x + segment, playerPos.y, playerPos.z);
    glm::vec3 leftPlayer_low = glm::vec3(playerPos.x + segment, playerPos.y - 1.59f, playerPos.z);
    glm::vec3 abovePlayer = glm::vec3(playerPos.x, playerPos.y + 0.2f, playerPos.z);

if (app->scene->world->voxel_handler->isVoxelSolid(belowPlayer)
or app->scene->world->voxel_handler->isVoxelSolid(belowPlayer1)
or app->scene->world->voxel_handler->isVoxelSolid(belowPlayer2)
or app->scene->world->voxel_handler->isVoxelSolid(belowPlayer3)
){
on_ground = true;  // Player has landed on the ground
velocity.y = 0;  // Reset vertical velocity
} else {
on_ground = false;
}

        //if (app->scene->world->voxel_handler->isVoxelSolid(belowPlayer)){
        //            on_ground = true;  // Player has landed on the ground
        //            velocity.y = 0;  // Reset vertical velocity
        //} else {
        //    on_ground = false;
        //}



    if (app->scene->world->voxel_handler->isVoxelSolid(frontPlayer) or app->scene->world->voxel_handler->isVoxelSolid(frontPlayer_low)) {
        if (velocity.z < 0) {
            velocity.z = 0;  // Reset vertical velocity
        }
    }
    if (app->scene->world->voxel_handler->isVoxelSolid(backPlayer) or app->scene->world->voxel_handler->isVoxelSolid(backPlayer_low)) {
        if (velocity.z > 0) {
            velocity.z = 0;  // Reset vertical velocity
        }
    }
    if (app->scene->world->voxel_handler->isVoxelSolid(rightPlayer) or app->scene->world->voxel_handler->isVoxelSolid(rightPlayer_low)) {
        if (velocity.x < 0) {
            velocity.x = 0;  // Reset vertical velocity}
        }
    }
    if (app->scene->world->voxel_handler->isVoxelSolid(leftPlayer) or app->scene->world->voxel_handler->isVoxelSolid(leftPlayer_low)) {
        if (velocity.x > 0) {
            velocity.x = 0;  // Reset vertical velocity
        }
    }
    if (app->scene->world->voxel_handler->isVoxelSolid(abovePlayer)) {
        if (velocity.y > 0) {
            velocity.y = 0;  // Reset vertical velocity
        }

    }


}

bool Player::isVoxelSolid(glm::vec3 voxelPos, std::array<std::array<float, CHUNK_VOL>, WORLD_VOL>& voxels_world) {
    if (voxelPos.x < 0 || voxelPos.y < 0 || voxelPos.z < 0 ||
    voxelPos.x >= WORLD_D * CHUNK_SIZE || voxelPos.y >= WORLD_H * CHUNK_SIZE || voxelPos.z >= WORLD_W * CHUNK_SIZE) {
        return false;  // Out of bounds is considered air
    }
    return true;
    //return voxels_world.at(voxelPos.x + WORLD_W * voxelPos.z + WORLD_AREA * voxelPos.y);
}

void Player::setFlyMode() {
    fly_mode = true;
}
void Player::setGravityMode() {
    fly_mode = false;
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
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::F)) {
        setFlyMode();
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::G)) {
        setGravityMode();
    }
    // Jumping (Spacebar)
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && on_ground && !fly_mode) {
        this->velocity.y = JUMP_STRENGTH;  // Set upward velocity to jump strength
        on_ground = false;  // Player is no longer on the ground
    }
}

void Player::keyboard_control_walk(float velocity) {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        this->velocity.x = velocity;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        this->velocity.x = -velocity;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        this->velocity.z = velocity;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        this->velocity.z = -velocity;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::F)) {
        setFlyMode();
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::G)) {
        setGravityMode();
    }
    // Jumping (Spacebar)
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && on_ground && !fly_mode) {
        this->velocity.y = JUMP_STRENGTH;  // Set upward velocity to jump strength
        on_ground = false;  // Player is no longer on the ground
    }
}

void Player::keyboard_block_type() {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1)) {
        app->scene->world->voxel_handler->newVoxelID = 1;
        std::cout << "Voxel ID: " << app->scene->world->voxel_handler->newVoxelID << std::endl;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2)) {
        app->scene->world->voxel_handler->newVoxelID = 2;
        std::cout << "Voxel ID: " << app->scene->world->voxel_handler->newVoxelID << std::endl;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3)) {
        app->scene->world->voxel_handler->newVoxelID = 3;
        std::cout << "Voxel ID: " << app->scene->world->voxel_handler->newVoxelID << std::endl;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num4)) {
        app->scene->world->voxel_handler->newVoxelID = 4;
        std::cout << "Voxel ID: " << app->scene->world->voxel_handler->newVoxelID << std::endl;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num5)) {
        app->scene->world->voxel_handler->newVoxelID = 5;
        std::cout << "Voxel ID: " << app->scene->world->voxel_handler->newVoxelID << std::endl;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num6)) {
        app->scene->world->voxel_handler->newVoxelID = 6;
        std::cout << "Voxel ID: " << app->scene->world->voxel_handler->newVoxelID << std::endl;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num7)) {
        app->scene->world->voxel_handler->newVoxelID = 7;
        std::cout << "Voxel ID: " << app->scene->world->voxel_handler->newVoxelID << std::endl;
    }
}