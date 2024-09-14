//
// Created by zzk on 2024/9/14.
//

#ifndef LIGHT_HPP
#define LIGHT_HPP

#include "settings.hpp"


class VoxelEngine;

class Light {
public:
    Light(VoxelEngine* app, glm::vec3 position, glm::vec3 color);
    glm::vec3 position;
    glm::vec3 color;
    glm::vec3 direction;

    glm::vec3 Ia;
    glm::vec3 Id;
    glm::vec3 Is;

    glm::mat4 m_view_light;
    glm::mat4 getViewMatrix();

    VoxelEngine* app;

    glm::mat4 getLightSpaceMatrix();
    void setDirection(const glm::vec3& dir);
};



#endif //LIGHT_HPP
