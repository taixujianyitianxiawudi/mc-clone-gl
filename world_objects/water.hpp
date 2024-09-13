//
// Created by zzk on 2024/9/12.
//

#ifndef WATER_HPP
#define WATER_HPP
#include "../voxel_engine.hpp"

class VoxelEngine;

class water {
public:
    water(VoxelEngine* app);
    void render();
    QuadMesh* mesh;
};



#endif //WATER_HPP
