//
// Created by zzk on 2024/9/13.
//

#include "cloud.hpp"

#include "../voxel_engine.hpp"

Cloud::Cloud(VoxelEngine* app) : app(app) {
    mesh = new CloudMesh(app);
}

void Cloud::render() {
    mesh->render();
}

void Cloud::update() {
    glUseProgram(mesh->program);
    app->shaderProgram->setFloatUniform(mesh->program, "u_time", app->time);
}


