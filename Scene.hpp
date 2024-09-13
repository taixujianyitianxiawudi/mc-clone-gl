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

class VoxelEngine;

class Scene {
public:
    Scene(VoxelEngine* app);

    void update();
    void render();
    World world;
    std::unique_ptr<VoxelMarker> voxel_marker;
private:
    VoxelEngine* app;
    //QuadMesh quad;


};



#endif //SCENE_HPP
