//
// Created by zzk on 2024/9/13.
//

#ifndef WATER_HPP
#define WATER_HPP
#include "../meshes/quad_mesh.hpp"
#include "../settings.hpp"

class VoxelEngine;

class Water {
public:
    Water(VoxelEngine* app);
    void render();
    QuadMesh* mesh;
    VoxelEngine* app;
};



#endif //WATER_HPP
