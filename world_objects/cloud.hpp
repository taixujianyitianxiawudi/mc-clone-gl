//
// Created by zzk on 2024/9/13.
//

#ifndef CLOUD_HPP
#define CLOUD_HPP
#include "../settings.hpp"
#include "../meshes/cloud_mesh.hpp"

class VoxelEngine;

class Cloud {
public:
    Cloud(VoxelEngine* app);
    void render();
    CloudMesh* mesh;
    VoxelEngine* app;
    void update();
};


#endif //CLOUD_HPP
