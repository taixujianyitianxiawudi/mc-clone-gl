//
// Created by zzk on 2024/9/13.
//

#include "water.hpp"

#include "../voxel_engine.hpp"

Water::Water(VoxelEngine* app) : app(app) {
    mesh = new QuadMesh(app);
}

void Water::render() {
    mesh->render();
}
