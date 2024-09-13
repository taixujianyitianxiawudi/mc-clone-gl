#ifndef VOXEL_MARKER_HPP
#define VOXEL_MARKER_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "../voxel_handler.hpp"
#include "../meshes/cube_mesh.hpp"  // Assuming you have a CubeMesh class

class VoxelMarker {
public:
    VoxelMarker(VoxelHandler* voxelHandler);

    void update();
    void setUniform();
    glm::mat4 getModelMatrix();
    void render();

private:
    VoxelHandler* handler;
    glm::vec3 position;
    glm::mat4 mModel;
    CubeMesh* mesh;
};

#endif // VOXEL_MARKER_HPP