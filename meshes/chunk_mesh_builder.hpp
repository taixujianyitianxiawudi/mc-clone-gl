#ifndef CHUNKMESHBUILDER_HPP
#define CHUNKMESHBUILDER_HPP

#include "../settings.hpp"

// Check if the voxel at the given position is void (empty)
//bool isVoid(const std::array<int, 3>& voxelPos, const std::vector<float>& chunkVoxels);
bool isVoid(const std::array<int, 3>& localVoxelPos,
    glm::vec3 worldVoxelPos,
    std::array<std::array<int, CHUNK_VOL>, WORLD_VOL>& worldVoxels);
// Helper function to add vertex data into the vertex data array
int addData(std::vector<int> &vertexData, int index, const std::array<std::array<int, 7>, 6>& vertices);

// Build the chunk mesh based on chunk voxels
std::vector<int> buildChunkMesh(
    const std::array<int, CHUNK_VOL>& chunkVoxels,
    int formatSize,
    glm::vec3 chunkPos,
    std::array<std::array<int, CHUNK_VOL>, WORLD_VOL>& worldVoxels
    );

int getChunkIndex(glm::vec3 world_voxel_pos);
#endif // CHUNKMESHBUILDER_HPP

std::array<int, 4> get_ao(const std::array<int, 3>& localVoxelPos, glm::vec3 worldVoxelPos,
                                      std::array<std::array<int, CHUNK_VOL>, WORLD_VOL>& worldVoxels, char plane);