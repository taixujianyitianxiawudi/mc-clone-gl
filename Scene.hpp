//
// Created by zzk on 2024/9/5.
//

#ifndef SCENE_HPP
#define SCENE_HPP
#include "settings.hpp"

#include "meshes/quad_mesh.hpp"
#include "world_objects/Chunk.hpp"
#include "World.hpp"
class VoxelEngine;

class Scene {
public:
    Scene(VoxelEngine* app);

    void update();
    void render();
private:
    VoxelEngine* app;
    //QuadMesh quad;
    World world;

};



#endif //SCENE_HPP
