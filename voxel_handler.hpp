#ifndef VOXEL_HANDLER_HPP
#define VOXEL_HANDLER_HPP

#include "settings.hpp"
#include "world.hpp"
#include "world_objects/chunk.hpp"

class VoxelHandler {
public:
    VoxelHandler(World* world);

    void addVoxel();
    void removeVoxel();
    void setVoxel();
    void switchMode();
    void update();
    bool rayCast();
    std::tuple<int, int, glm::ivec3, Chunk*> getVoxelID(const glm::ivec3& voxelWorldPos);
    World* world;
private:


    std::array<Chunk*, WORLD_VOL> chunks;
    //std::vector<Chunk*> chunks;

    Chunk* chunk;
    int voxelID;
    int voxelIndex;
    glm::ivec3 voxelLocalPos;
    glm::ivec3 voxelWorldPos;
    glm::ivec3 voxelNormal;

    bool interactionMode;
    int newVoxelID;

    void rebuildAdjacentChunks();
    void rebuildAdjChunk(const glm::ivec3& adjVoxelPos);
    int get_chunk_index(const glm::ivec3& adjVoxelPos);

    friend class VoxelMarker;
};

#endif // VOXEL_HANDLER_HPP