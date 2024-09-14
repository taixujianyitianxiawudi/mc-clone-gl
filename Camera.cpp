#include "Camera.hpp"

// Constructor
Camera::Camera(const glm::vec3& position, float yaw, float pitch)
    : position(position), yaw(glm::radians(yaw)), pitch(glm::radians(pitch)), up(glm::vec3(0, 1, 0)),
      right(glm::vec3(1, 0, 0)), forward(glm::vec3(0, 0, -1)) {
    // Set the perspective projection matrix using values from settings.hpp
    m_proj = glm::perspective(V_FOV, ASPECT_RATIO, NEAR, FAR);
    m_view = glm::mat4(1.0f);
    frustum = new Frustum(this);
    update();
}

// Update both vectors and view matrix
void Camera::update() {
    update_vectors();
    update_view_matrix();
}

// Update view matrix based on the position and orientation of the camera
void Camera::update_view_matrix() {
    m_view = glm::lookAt(position, position + forward, up);
}

// Update camera vectors based on yaw and pitch
void Camera::update_vectors() {
    forward.x = glm::cos(yaw) * glm::cos(pitch);
    forward.y = glm::sin(pitch);
    forward.z = glm::sin(yaw) * glm::cos(pitch);

    forward = glm::normalize(forward);
    right = glm::normalize(glm::cross(forward, glm::vec3(0.0f, 1.0f, 0.0f)));
    up = glm::normalize(glm::cross(right, forward));
}

// Rotate pitch based on input
void Camera::rotate_pitch(float delta_y) {
    pitch -= delta_y;
    pitch = glm::clamp(pitch, -PITCH_MAX, PITCH_MAX);  // Clamping pitch to avoid gimbal lock
}

// Rotate yaw based on input
void Camera::rotate_yaw(float delta_x) {
    yaw += delta_x;
}

// Movement functions for the camera
void Camera::move_left(float velocity) {
    position -= right * velocity;
}

void Camera::move_right(float velocity) {
    position += right * velocity;
}

void Camera::move_up(float velocity) {
    position += glm::vec3(0.0f, 1.0f, 0.0f) * velocity;
}

void Camera::move_down(float velocity) {
    position -= glm::vec3(0.0f, 1.0f, 0.0f) * velocity;
}

void Camera::move_forward(float velocity) {
    position += forward * velocity;
}

void Camera::move_back(float velocity) {
    position -= forward * velocity;
}

