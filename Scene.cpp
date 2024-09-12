//
// Created by zzk on 2024/9/5.
//

#include "Scene.hpp"
#include "voxel_engine.hpp"

// Constructor
Scene::Scene(VoxelEngine* app)
    : app(app),
    //quad(app),
    world(app) {}

// Update function (currently empty)
void Scene::update() {
    world.update();
    // Update the scene here if needed
}

// Render function to render the quad mesh
void Scene::render() {
    //quad.render();
    world.render();
}