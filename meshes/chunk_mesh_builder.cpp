#include "chunk_mesh_builder.hpp"
#include "../world_objects/chunk.hpp"
#include "../World.hpp"

void ChunkMeshBuilder::buildMesh(Chunk* chunk, std::vector<float>& vertexData) {
    std::array<float, CHUNK_VOL>& voxels = *chunk->voxels_chunk;

    for (int x = 0; x < CHUNK_SIZE; ++x) {
        for (int y = 0; y < CHUNK_SIZE; ++y) {
            for (int z = 0; z < CHUNK_SIZE; ++z) {
                int voxel_id = voxels[x + CHUNK_SIZE * z + CHUNK_AREA * y];

                if (voxel_id != 0) {  // Only process non-empty voxels
                    // Add faces only if the neighboring voxel is air or outside the chunk bounds
                    if (isFaceExposed(chunk, x, y + 1, z))  // Top face
                        addFace(vertexData, glm::ivec3(x, y, z), 0, voxel_id);
                    if (isFaceExposed(chunk, x, y - 1, z))  // Bottom face
                        addFace(vertexData, glm::ivec3(x, y, z), 1, voxel_id);
                    if (isFaceExposed(chunk, x + 1, y, z))  // Right face
                        addFace(vertexData, glm::ivec3(x, y, z), 2, voxel_id);
                    if (isFaceExposed(chunk, x - 1, y, z))  // Left face
                        addFace(vertexData, glm::ivec3(x, y, z), 3, voxel_id);
                    if (isFaceExposed(chunk, x, y, z + 1))  // Front face
                        addFace(vertexData, glm::ivec3(x, y, z), 4, voxel_id);
                    if (isFaceExposed(chunk, x, y, z - 1))  // Back face
                        addFace(vertexData, glm::ivec3(x, y, z), 5, voxel_id);
                }
            }
        }
    }
}

bool ChunkMeshBuilder::isFaceExposed(Chunk* chunk, int x, int y, int z) {
    // If the voxel is outside the chunk bounds, check the adjacent chunk
    if (x < 0 || x >= CHUNK_SIZE || y < 0 || y >= CHUNK_SIZE || z < 0 || z >= CHUNK_SIZE) {
        return isNeighborVoxelEmpty(chunk, x, y, z);  // Check adjacent chunk
    }

    // Inside the chunk, just check if the voxel is empty
    int index = x + CHUNK_SIZE * z + CHUNK_AREA * y;
    return chunk->voxels_chunk->at(index) == 0;  // Return true if the voxel is empty (air)
}

// This function checks if a neighboring chunk has an empty voxel at (x, y, z)
bool ChunkMeshBuilder::isNeighborVoxelEmpty(Chunk* chunk, int x, int y, int z) {
    // Get the neighboring chunk based on which side we're checking
    glm::ivec3 neighborChunkPos = chunk->position;

    if (x < 0) {
        neighborChunkPos.x -= 1;
        x = CHUNK_SIZE - 1;
    } else if (x >= CHUNK_SIZE) {
        neighborChunkPos.x += 1;
        x = 0;
    }

    if (y < 0) {
        neighborChunkPos.y -= 1;
        y = CHUNK_SIZE - 1;
    } else if (y >= CHUNK_SIZE) {
        neighborChunkPos.y += 1;
        y = 0;
    }

    if (z < 0) {
        neighborChunkPos.z -= 1;
        z = CHUNK_SIZE - 1;
    } else if (z >= CHUNK_SIZE) {
        neighborChunkPos.z += 1;
        z = 0;
    }

    // Fetch the neighboring chunk from the world
    Chunk* neighborChunk = chunk->world->getChunkAt(neighborChunkPos);
    if (neighborChunk == nullptr || neighborChunk->is_empty) {
        // If there's no neighboring chunk or it's empty, consider the face exposed
        return true;
    }

    // Check if the voxel in the neighboring chunk is empty
    int neighborIndex = x + CHUNK_SIZE * z + CHUNK_AREA * y;
    return neighborChunk->voxels_chunk->at(neighborIndex) == 0;
}

void ChunkMeshBuilder::addFace(std::vector<float>& vertexData, const glm::ivec3& pos, int face, int voxel_id) {
    static const glm::vec3 faceVertices[6][4] = {
        // Top face (Y+)
        { {0, 1, 0}, {1, 1, 0}, {1, 1, 1}, {0, 1, 1} },
        // Bottom face (Y-)
        { {0, 0, 0}, {1, 0, 0}, {1, 0, 1}, {0, 0, 1} },
        // Right face (X+)
        { {1, 0, 0}, {1, 1, 0}, {1, 1, 1}, {1, 0, 1} },
        // Left face (X-)
        { {0, 0, 0}, {0, 1, 0}, {0, 1, 1}, {0, 0, 1} },
        // Front face (Z+)
        { {0, 0, 1}, {1, 0, 1}, {1, 1, 1}, {0, 1, 1} },
        // Back face (Z-)
        { {0, 0, 0}, {1, 0, 0}, {1, 1, 0}, {0, 1, 0} }
    };

    const glm::vec3* verts = faceVertices[face];

    // First triangle (0, 1, 2)
    for (int i = 0; i < 3; ++i) {
        const glm::vec3& vertex = verts[i];
        vertexData.push_back(static_cast<float>(pos.x) + vertex.x);
        vertexData.push_back(static_cast<float>(pos.y) + vertex.y);
        vertexData.push_back(static_cast<float>(pos.z) + vertex.z);

        glm::vec2 uv = getUV(face, i);
        vertexData.push_back(uv.x);
        vertexData.push_back(uv.y);

        vertexData.push_back(voxel_id);
    }

    // Second triangle (0, 2, 3)
    for (int i = 0; i < 3; ++i) {
        const glm::vec3& vertex = verts[i == 0 ? 0 : i + 1];
        vertexData.push_back(static_cast<float>(pos.x) + vertex.x);
        vertexData.push_back(static_cast<float>(pos.y) + vertex.y);
        vertexData.push_back(static_cast<float>(pos.z) + vertex.z);

        glm::vec2 uv = getUV(face, i == 0 ? 0 : i + 1);
        vertexData.push_back(uv.x);
        vertexData.push_back(uv.y);

        vertexData.push_back(voxel_id);
    }
}

glm::vec2 ChunkMeshBuilder::getUV(int face, int vertexIndex) {
    // Hard-coded UV coordinates for each face vertex
    static const glm::vec2 uvs[4] = {
        { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f }
    };

    return uvs[vertexIndex % 4];  // Return UV based on vertex index (0 to 3)
}