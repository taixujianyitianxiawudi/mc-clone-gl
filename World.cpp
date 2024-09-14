#include "World.hpp"
#include "../settings.hpp"

// Constructor: Initializes the world
World::World(VoxelEngine* app)
    : app(app)
{
    buildChunks();
    //saveChunkToDisk(voxels_world, "world.bin");
    //loadChunks();
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

void World::loadChunks() {
    loadChunkFromDisk(voxels_world, "world.bin");
    for (int x = 0; x < WORLD_W; ++x) {
        for (int y = 0; y < WORLD_H; ++y) {
            for (int z = 0; z < WORLD_D; ++z) {
                // Create a new chunk at position (x, y, z)
                Chunk* chunk = new Chunk(this, glm::vec3(x, y, z));

                // Calculate chunk index in 1D array
                int chunkIndex = x + WORLD_W * z + WORLD_AREA * y;

                // Set the chunk at this index
                chunks[chunkIndex] = chunk;

                // get pointer to voxels
                chunk->voxels_chunk = &voxels_world[chunkIndex];
                chunk->check_empty();
            }
        }
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

void World::render_with_shader_program(GLuint program) {
    for (auto& chunk : chunks) {
        chunk->render_with_shader_program(program);  // Render each chunk
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

void World::saveChunkToDisk(std::array<std::array<float, CHUNK_VOL>, WORLD_VOL> &voxels_world,
    const std::string &filename) {
        std::ofstream outFile(filename, std::ios::binary);
        if (!outFile) {
            std::cerr << "Failed to open file for writing: " << filename << std::endl;
            return;
        }

        // Write the entire world array to the file as raw binary data
        for (const auto& chunk : voxels_world) {
            outFile.write(reinterpret_cast<const char*>(chunk.data()), chunk.size() * sizeof(float));
        }

        outFile.close();
        std::cout << "World saved to " << filename << std::endl;
}

void World::loadChunkFromDisk(std::array<std::array<float, CHUNK_VOL>, WORLD_VOL> &voxels_world,
    const std::string &filename) {

    std::ifstream inFile(filename, std::ios::binary);
    if (!inFile) {
        std::cerr << "Failed to open file for reading: " << filename << std::endl;
        return;
    }

    // Read the entire world array from the file as raw binary data
    for (auto& chunk : voxels_world) {
        inFile.read(reinterpret_cast<char*>(chunk.data()), chunk.size() * sizeof(float));
    }

    inFile.close();
    std::cout << "World loaded from " << filename << std::endl;
}
