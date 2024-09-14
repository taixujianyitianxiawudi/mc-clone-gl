#include "voxel_handler.hpp"
#include "world_objects/chunk.hpp"
#include <glm/gtc/noise.hpp>
#include <algorithm>
#include "voxel_engine.hpp"
VoxelHandler::VoxelHandler(World* world)
    : world(world), chunks(world->chunks), chunk(nullptr), voxelID(0),
      voxelIndex(0), interactionMode(false), newVoxelID(1) {}

void VoxelHandler::addVoxel() {
    if (voxelID) {
        auto result = getVoxelID(voxelWorldPos + voxelNormal);
        if (std::get<0>(result) == 0) {
            int voxelIndex = std::get<1>(result);
            Chunk* chunk = std::get<3>(result);
            chunk->voxels_chunk->at(voxelIndex) = static_cast<float>(newVoxelID);
            chunk->mesh->rebuild();
            if (chunk->is_empty) {
                chunk->is_empty = false;
            }
        }
    }
}

void VoxelHandler::removeVoxel() {
    if (voxelID) {
        chunk->voxels_chunk->at(voxelIndex) = 0;
        chunk->mesh->rebuild();
        rebuildAdjacentChunks();
    }
}

void VoxelHandler::setVoxel() {
    if (interactionMode) {
        addVoxel();
    } else {
        removeVoxel();
    }
}

void VoxelHandler::switchMode() {
    interactionMode = !interactionMode;
}

void VoxelHandler::update() {
    rayCast();
}

bool VoxelHandler::rayCast() {
    glm::vec3 start = world->getApp()->player->get_position();
    glm::vec3 end = start + world->getApp()->player->get_forward() * static_cast<float>(MAX_RAY_DIST);

    glm::ivec3 currentVoxelPos = glm::floor(start);
    voxelID = 0;
    voxelNormal = glm::ivec3(0);
    int stepDir = -1;

    float dx = glm::sign(end.x - start.x);
    float deltaX = (dx != 0) ? std::min(dx / (end.x - start.x), 10000000.0f) : 10000000.0f;
    float maxX = (dx > 0) ? deltaX * (1.0f - glm::fract(start.x)) : deltaX * glm::fract(start.x);

    float dy = glm::sign(end.y - start.y);
    float deltaY = (dy != 0) ? std::min(dy / (end.y - start.y), 10000000.0f) : 10000000.0f;
    float maxY = (dy > 0) ? deltaY * (1.0f - glm::fract(start.y)) : deltaY * glm::fract(start.y);

    float dz = glm::sign(end.z - start.z);
    float deltaZ = (dz != 0) ? std::min(dz / (end.z - start.z), 10000000.0f) : 10000000.0f;
    float maxZ = (dz > 0) ? deltaZ * (1.0f - glm::fract(start.z)) : deltaZ * glm::fract(start.z);

    while (!(maxX > 1.0f && maxY > 1.0f && maxZ > 1.0f)) {
        auto result = getVoxelID(currentVoxelPos);
        if (std::get<0>(result)) {
            voxelID = std::get<0>(result);
            voxelIndex = std::get<1>(result);
            voxelLocalPos = std::get<2>(result);
            chunk = std::get<3>(result);
            voxelWorldPos = currentVoxelPos;

            if (stepDir == 0) voxelNormal.x = -dx;
            else if (stepDir == 1) voxelNormal.y = -dy;
            else voxelNormal.z = -dz;
            return true;
        }

        if (maxX < maxY) {
            if (maxX < maxZ) {
                currentVoxelPos.x += dx;
                maxX += deltaX;
                stepDir = 0;
            } else {
                currentVoxelPos.z += dz;
                maxZ += deltaZ;
                stepDir = 2;
            }
        } else {
            if (maxY < maxZ) {
                currentVoxelPos.y += dy;
                maxY += deltaY;
                stepDir = 1;
            } else {
                currentVoxelPos.z += dz;
                maxZ += deltaZ;
                stepDir = 2;
            }
        }
    }
    return false;
}

std::tuple<int, int, glm::ivec3, Chunk*> VoxelHandler::getVoxelID(const glm::ivec3& voxelWorldPos) {
    glm::ivec3 chunkPos = voxelWorldPos / CHUNK_SIZE;

    if (chunkPos.x >= 0 && chunkPos.x < WORLD_W && chunkPos.y >= 0 && chunkPos.y < WORLD_H && chunkPos.z >= 0 && chunkPos.z < WORLD_D) {
        int chunkIndex = chunkPos.x + WORLD_W * chunkPos.z + WORLD_AREA * chunkPos.y;
        Chunk* chunk = chunks[chunkIndex];

        glm::ivec3 voxelLocalPos = voxelWorldPos - chunkPos * CHUNK_SIZE;

        int voxelIndex = voxelLocalPos.x + CHUNK_SIZE * voxelLocalPos.z + CHUNK_AREA * voxelLocalPos.y;
        int voxelID = chunk->voxels_chunk->at(voxelIndex);

        return {voxelID, voxelIndex, voxelLocalPos, chunk};
    }
    return {0, 0, glm::ivec3(0), nullptr};
}

void VoxelHandler::rebuildAdjChunk(const glm::ivec3& adjVoxelPos) {
    int index = get_chunk_index(adjVoxelPos);  // Assuming get_chunk_index is implemented elsewhere
    if (index != -1) {
        chunks[index]->mesh->rebuild();
    }
}



int VoxelHandler::get_chunk_index(const glm::ivec3& adjVoxelPos) {
    int wx = adjVoxelPos.x;
    int wy = adjVoxelPos.y;
    int wz = adjVoxelPos.z;
    int cx = wx / CHUNK_SIZE;
    int cy = wy / CHUNK_SIZE;
    int cz = wz / CHUNK_SIZE;
    //if not (0 <= cx < WORLD_W and 0 <= cy < WORLD_H and 0 <= cz < WORLD_D):
    //    return -1;
    if (cx < 0 || cx >= WORLD_W || cy < 0 || cy >= WORLD_H || cz < 0 || cz >= WORLD_D) {
        return -1;
    }

    int index = cx + WORLD_W * cz + WORLD_AREA * cy;
    return index;
}


void VoxelHandler::rebuildAdjacentChunks() {
    int lx = voxelLocalPos.x, ly = voxelLocalPos.y, lz = voxelLocalPos.z;
    int wx = voxelWorldPos.x, wy = voxelWorldPos.y, wz = voxelWorldPos.z;

    if (lx == 0) rebuildAdjChunk(glm::ivec3(wx - 1, wy, wz));
    else if (lx == CHUNK_SIZE - 1) rebuildAdjChunk(glm::ivec3(wx + 1, wy, wz));

    if (ly == 0) rebuildAdjChunk(glm::ivec3(wx, wy - 1, wz));
    else if (ly == CHUNK_SIZE - 1) rebuildAdjChunk(glm::ivec3(wx, wy + 1, wz));

    if (lz == 0) rebuildAdjChunk(glm::ivec3(wx, wy, wz - 1));
    else if (lz == CHUNK_SIZE - 1) rebuildAdjChunk(glm::ivec3(wx, wy, wz + 1));
}

bool VoxelHandler::isVoxelSolid(const glm::ivec3& voxelWorldPos) {
    glm::ivec3 chunkPos = voxelWorldPos / CHUNK_SIZE;

    // Check if the chunk is within the world bounds
    if (chunkPos.x >= 0 && chunkPos.x < WORLD_W &&
        chunkPos.y >= 0 && chunkPos.y < WORLD_H &&
        chunkPos.z >= 0 && chunkPos.z < WORLD_D) {

        // Get the chunk index
        int chunkIndex = chunkPos.x + WORLD_W * chunkPos.z + WORLD_AREA * chunkPos.y;
        Chunk* chunk = chunks[chunkIndex];

        if (!chunk) return false;  // If the chunk doesn't exist, treat it as air (not solid)

        // Calculate the local voxel position within the chunk
        glm::ivec3 voxelLocalPos = voxelWorldPos - chunkPos * CHUNK_SIZE;

        // Calculate the voxel index in the chunk's voxel array
        int voxelIndex = voxelLocalPos.x + CHUNK_SIZE * voxelLocalPos.z + CHUNK_AREA * voxelLocalPos.y;

        // Check if the voxel is solid (non-air, e.g., non-zero)
        int voxelID = chunk->voxels_chunk->at(voxelIndex);
        return voxelID != 0;  // Return true if the voxel is solid, otherwise false
        }

    // If the voxel is out of bounds, treat it as air (not solid)
    return false;
}