//
// Created by zzk on 2024/9/14.
//

#include "light.hpp"
#include "voxel_engine.hpp"

Light::Light(VoxelEngine* app, glm::vec3 position, glm::vec3 color):
position(position), app(app), color(color), direction(0.0f, 0.0f,0.0f){
    Ia = 0.06f * color;  // Ambient intensity
    Id = 0.8f * color;   // Diffuse intensity
    Is = 1.0f * color;   // Specular intensity

    // Generate the view matrix for the light
    m_view_light = getViewMatrix();
}

glm::mat4 Light::getViewMatrix() {
    return glm::lookAt(position, direction, glm::vec3(0.0f, 1.0f, 0.0f));  // Look at the origin
}

glm::mat4 Light::getLightSpaceMatrix() {
    // Light's view matrix
    glm::mat4 lightViewMatrix = getViewMatrix();

    // Light's orthographic projection matrix
    float near_plane = 1.0f, far_plane = 100.0f;
    float left = -50.0f, right = 50.0f, bottom = -50.0f, top = 50.0f;
    glm::mat4 lightProjectionMatrix = glm::ortho(left, right, bottom, top, near_plane, far_plane);

    // Combine projection and view matrices
    return lightProjectionMatrix * lightViewMatrix;
}

void Light::setDirection(const glm::vec3& dir) {
    direction = dir;
    m_view_light = getViewMatrix();  // Recalculate view matrix
}