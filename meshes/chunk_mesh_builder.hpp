#ifndef CHUNK_MESH_BUILDER_HPP
#define CHUNK_MESH_BUILDER_HPP

#include "../settings.hpp"  // For CHUNK_SIZE, etc.
class Chunk;

class ChunkMeshBuilder {
public:
    // Function to build the mesh for a chunk
    static void buildMesh(Chunk* chunk, std::vector<float>& vertexData);

private:
    // Internal function to add a face to the mesh data
    static void addFace(std::vector<float>& vertexData, const glm::ivec3& pos, int face, int voxel_id);
    static bool isFaceExposed(Chunk* chunk, int x, int y, int z);
    // Internal function to generate UVs
    static glm::vec2 getUV(int face, int vertexIndex);
    static bool isNeighborVoxelEmpty(Chunk* chunk, int x, int y, int z);
    static glm::vec2 getUV_(int face, int vertexIndex);
};

#endif // CHUNK_MESH_BUILDER_HPP
