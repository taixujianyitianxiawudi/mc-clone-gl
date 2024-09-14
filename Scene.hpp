//
// Created by zzk on 2024/9/5.
//

#ifndef SCENE_HPP
#define SCENE_HPP
#include "settings.hpp"

#include "meshes/quad_mesh.hpp"
#include "world_objects/Chunk.hpp"
#include "World.hpp"
#include "world_objects/VoxelMarker.hpp"
#include "world_objects/water.hpp"
#include "world_objects/cloud.hpp"
#include "light.hpp"
class VoxelEngine;

class Scene {
public:
    Scene(VoxelEngine* app);

    void update();
    void render();
    void render_new();
    World* world;
    Water* water;
    Light* light;
    Cloud* cloud;
    std::unique_ptr<VoxelMarker> voxel_marker;
private:
    VoxelEngine* app;
    //QuadMesh quad;


};



#endif //SCENE_HPP
