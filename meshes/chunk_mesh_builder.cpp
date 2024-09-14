#include "chunk_mesh_builder.hpp"
#include "../world_objects/chunk.hpp"
#include "../World.hpp"

void ChunkMeshBuilder::buildMesh(Chunk* chunk, std::vector<float>& vertexData) {
    std::array<float, CHUNK_VOL>& voxels = *chunk->voxels_chunk;

    for (int x = 0; x < CHUNK_SIZE; ++x) {
        for (int y = 0; y < CHUNK_SIZE; ++y) {
            for (int z = 0; z < CHUNK_SIZE; ++z) {
                int index = x + CHUNK_SIZE * z + CHUNK_AREA * y;
                int voxel_id = voxels[index];

                if (voxel_id == 0) continue;  // Skip empty voxels

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
        //{ {0, 1, 0}, {1, 1, 0}, {1, 1, 1}, {0, 1, 1} },
        { {0, 1, 1} ,{1, 1, 1},{1, 1, 0}, {0, 1, 0}, }, // reversed top
        // Bottom face (Y-)
        { {0, 0, 0}, {1, 0, 0}, {1, 0, 1}, {0, 0, 1} },
        // Right face (X+)
        { {1, 0, 0}, {1, 1, 0}, {1, 1, 1}, {1, 0, 1} },
        // Left face (X-)
        //{ {0, 0, 0}, {0, 1, 0}, {0, 1, 1}, {0, 0, 1} },
        { {0, 0, 1} ,{0, 1, 1}, {0, 1, 0}, {0, 0, 0} }, // reversed left
        // Front face (Z+)
        { {0, 0, 1}, {1, 0, 1}, {1, 1, 1}, {0, 1, 1} },
        // Back face (Z-)
        //{ {0, 0, 0}, {1, 0, 0}, {1, 1, 0}, {0, 1, 0} }
        { {0, 1, 0}, {1, 1, 0}, {1, 0, 0}, {0, 0, 0}, }  // reversed back
    };

    const glm::vec3* verts = faceVertices[face];

    // First triangle (0, 1, 2)
    for (int i = 0; i < 3; ++i) {
    //for (int i = 3; i > 0; i--) {
        const glm::vec3& vertex = verts[i];
        vertexData.push_back(static_cast<float>(pos.x) + vertex.x);
        vertexData.push_back(static_cast<float>(pos.y) + vertex.y);
        vertexData.push_back(static_cast<float>(pos.z) + vertex.z);

        glm::vec2 uv = getUV_(face, i);
        vertexData.push_back(uv.x);
        vertexData.push_back(uv.y);

        vertexData.push_back(static_cast<float>(voxel_id));
        vertexData.push_back(static_cast<float>(face));
    }

    // Second triangle (0, 2, 3)
    for (int i = 0; i < 3; ++i) {
    //for (int i = 3; i > 0; i--) {
        const glm::vec3& vertex = verts[i == 0 ? 0 : i + 1];
        vertexData.push_back(static_cast<float>(pos.x) + vertex.x);
        vertexData.push_back(static_cast<float>(pos.y) + vertex.y);
        vertexData.push_back(static_cast<float>(pos.z) + vertex.z);

        glm::vec2 uv = getUV_(face, i == 0 ? 0 : i + 1);
        vertexData.push_back(uv.x);
        vertexData.push_back(uv.y);

        vertexData.push_back(static_cast<float>(voxel_id));
        vertexData.push_back(static_cast<float>(face));
    }
}

glm::vec2 ChunkMeshBuilder::getUV_(int face, int vertexIndex) {
    // Define UV coordinates for each face
    static const glm::vec2 uvTop[4] = {
        { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f }
    };

    static const glm::vec2 uvBottom[4] = {
        { 0.0f, 1.0f }, { 1.0f, 1.0f }, { 1.0f, 0.0f }, { 0.0f, 0.0f }
    };

    static const glm::vec2 uvSide[4] = {
        { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f }
    };

    static const glm::vec2 uvRotated[4] = {
        { 0.0f, 1.0f }, { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }
    };

    // For front and back faces, flip the vertical coordinates
    static const glm::vec2 uvFlippedSide[4] = {
        { 0.0f, 1.0f }, { 1.0f, 1.0f }, { 1.0f, 0.0f }, { 0.0f, 0.0f }
    };

    switch (face) {
        case 0: // Top face (Y+)
            return uvTop[vertexIndex % 4];
        case 1: // Bottom face (Y-)
            return uvBottom[vertexIndex % 4];
        case 2: // Right face (X+)
        case 3: // Left face (X-)
            return uvRotated[vertexIndex % 4];  // Use rotated UVs for right and left faces
        case 4: // Front face (Z+)
            return uvFlippedSide[vertexIndex % 4];  // No flipping for the front face
        case 5: // Back face (Z-)
            return uvSide[vertexIndex % 4];  // No flipping for the front face
        default:
            return { 0.0f, 0.0f };  // Fallback UV, should not happen
    }
}

glm::vec2 ChunkMeshBuilder::getUV(int face, int vertexIndex) {
    // UV coordinates for top, bottom, side, rotated, and flipped faces
    static const glm::vec2 uvMappings[5][4] = {
        { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } },  // Top, side
        { { 0.0f, 1.0f }, { 1.0f, 1.0f }, { 1.0f, 0.0f }, { 0.0f, 0.0f } },  // Bottom
        { { 0.0f, 1.0f }, { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f } },  // Rotated
        { { 0.0f, 1.0f }, { 1.0f, 1.0f }, { 1.0f, 0.0f }, { 0.0f, 0.0f } }   // Flipped
    };

    // Face mappings: 0 -> top, 1 -> bottom, 2-3 -> rotated, 4-5 -> flipped
    int uvType = (face == 0) ? 0 : (face == 1) ? 1 : (face <= 3) ? 2 : 3;
    return uvMappings[uvType][vertexIndex % 4];
}