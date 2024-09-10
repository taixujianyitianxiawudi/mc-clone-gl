#include "World.hpp"
#include "../settings.hpp"

// Constructor: Initializes the world
World::World(VoxelEngine* app)
    : app(app)
{

    // no need to do this
    // Initialize voxels to zero (or other values as needed)
    //for (auto& chunk_voxels : voxels) {
    //    chunk_voxels.fill(0);  // Fill each chunk's voxel array with 0
    //}

    buildChunks();
    buildChunkMesh();
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
                voxels[chunkIndex] = chunks[chunkIndex]->buildVoxels();

                // get pointer to voxels
                chunk->voxels = &voxels[chunkIndex];

            }
        }
    }
}

// Function to build the chunk meshes
void World::buildChunkMesh() {
    for (const auto& chunk : chunks) {
        chunk->buildMesh();  // Build the mesh for each chunk
    }
}

// Update function (currently does nothing)
void World::update() {
    // Update logic for the world can be added here
}

// Render function: Renders all the chunks
void World::render() {
    for (auto& chunk : chunks) {
        chunk->render();  // Render each chunk
    }
    //for (int i = 0; i < WORLD_VOL; i++) {
    //    chunks[i]->render();
    //}
}

VoxelEngine * World::getApp() {
    return app;
}
