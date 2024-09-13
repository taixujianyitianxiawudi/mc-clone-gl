//
// Created by zzk on 2024/9/5.
//

#ifndef CAMERA_HPP
#define CAMERA_HPP
#include "frustum.hpp"
#include "settings.hpp"


class Camera {
public:
    Camera(const glm::vec3& position, float yaw, float pitch);

    // Update camera vectors and view matrix
    void update();

    // Movement functions
    void move_left(float velocity);
    void move_right(float velocity);
    void move_up(float velocity);
    void move_down(float velocity);
    void move_forward(float velocity);
    void move_back(float velocity);

    // Rotation functions
    void rotate_pitch(float delta_y);
    void rotate_yaw(float delta_x);

    // Getters
    glm::mat4 get_view_matrix() const { return m_view; }
    glm::mat4 get_projection_matrix() const { return m_proj; }

    glm::vec3 get_position() const { return position; }
    glm::vec3 get_forward() const { return forward; }
    glm::vec3 get_right() const { return right; }
    glm::vec3 get_up() const { return up; }

    Frustum* frustum;
private:
    // Camera attr
    // ibutes
    glm::vec3 position;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 forward;

    float yaw;
    float pitch;

    glm::mat4 m_proj;
    glm::mat4 m_view;

    // Internal helper functions
    void update_view_matrix();
    void update_vectors();


};

#endif // CAMERA_HPP
