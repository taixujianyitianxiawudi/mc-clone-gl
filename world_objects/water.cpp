//
// Created by zzk on 2024/9/12.
//

#include "water.hpp"

water::water(VoxelEngine *app) {
    mesh = new QuadMesh(app);
}

void water::render() {
    mesh->render();
}

