#ifndef FRUSTUM_HPP
#define FRUSTUM_HPP

#include <glm/glm.hpp>

#include "world_objects/chunk.hpp"
#include "settings.hpp"
class Camera;

class Frustum {
public:
    // Constructor
    Frustum(Camera* camera);

    // Function to check if a chunk is within the frustum
    bool isOnFrustum(Chunk* chunk);

private:
    Camera* cam;  // Pointer to the Camera object

    float factorY;
    float tanY;
    float factorX;
    float tanX;

};


#endif // FRUSTUM_HPP