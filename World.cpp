#include "World.hpp"
#include "../settings.hpp"

// Constructor: Initializes the world
World::World(VoxelEngine* app)
    : app(app)
{
    buildChunks();
    buildChunkMesh();
    voxel_handler = new VoxelHandler(this);
}

// Function to build the chunks
void World::buildChunks() {
    for (int x = 0; x < WORLD_W; ++x) {
        for (int y = 0; y < WORLD_H; ++y) {
            for (int z = 0; z < WORLD_D; ++z) {
                // Create a new chunk at position (x, y, z)
                Chunk* chunk = new Chunk(this, glm::vec3(x, y, z));

                // Calculate chunk index in 1D array
                int chunkIndex = x + WORLD_W * z + WORLD_AREA * y;

                // Set the chunk at this index
                chunks[chunkIndex] = chunk;

                // Build the voxels for this chunk and store them in the voxel array
                //voxels_world[chunkIndex] = chunks[chunkIndex]->buildVoxels();
                voxels_world[chunkIndex] = chunks[chunkIndex]->buildVoxels_new();
                // get pointer to voxels
                chunk->voxels_chunk = &voxels_world[chunkIndex];

            }
        }
    }
}

// Function to build the chunk meshes
void World::buildChunkMesh() {
    for (const auto& chunk : chunks) {
        chunk->buildMesh();  // Build the mesh for each chunk
        chunk->mesh->rebuild();
    }
}

// Update function (currently does nothing)
void World::update() {
    voxel_handler->update();
    // Update logic for the world can be added here
}

// Render function: Renders all the chunks
void World::render() {
    for (auto& chunk : chunks) {
        chunk->render();  // Render each chunk
    }
}

VoxelEngine * World::getApp() {
    return app;
}

Chunk* World::getChunkAt(const glm::ivec3& chunkPos) const {
    if (chunkPos.x < 0 || chunkPos.x >= WORLD_W ||
        chunkPos.y < 0 || chunkPos.y >= WORLD_H ||
        chunkPos.z < 0 || chunkPos.z >= WORLD_D) {
        return nullptr;  // Outside world bounds
        }

    int chunkIndex = chunkPos.x + WORLD_W * chunkPos.z + WORLD_AREA * chunkPos.y;
    return chunks[chunkIndex];
}
