#include "frustum.hpp"
#include <glm/glm.hpp>
#include <cmath>
#include "Camera.hpp"

// Constructor: initialize the frustum properties based on the camera's field of view
Frustum::Frustum(Camera* camera) : cam(camera) {
    float halfY = V_FOV * 0.5f;
    tanY = std::tan(halfY);
    factorY = 1.0f / std::cos(halfY);

    float halfX = H_FOV * 0.5f;
    tanX = std::tan(halfX);
    factorX = 1.0f / std::cos(halfX);
}

// Check if the chunk is within the frustum
bool Frustum::isOnFrustum(Chunk* chunk) {
    // Vector to the sphere center of the chunk
    glm::vec3 sphereVec = chunk->center - cam->get_position();

    // Outside the NEAR and FAR planes?
    float sz = glm::dot(sphereVec, cam->get_forward());
    if (!(NEAR - CHUNK_SPHERE_RADIUS <= sz && sz <= FAR + CHUNK_SPHERE_RADIUS)) {
        return false;
    }

    // Outside the TOP and BOTTOM planes?
    float sy = glm::dot(sphereVec, cam->get_up());
    float distY = factorY * CHUNK_SPHERE_RADIUS + sz * tanY;
    if (!(sy >= -distY && sy <= distY)) {
        return false;
    }

    // Outside the LEFT and RIGHT planes?
    float sx = glm::dot(sphereVec, cam->get_right());
    float distX = factorX * CHUNK_SPHERE_RADIUS + sz * tanX;
    if (!(sx >= -distX && sx <= distX)) {
        return false;
    }

    return true;
}