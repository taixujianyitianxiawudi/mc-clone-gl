#include <set>

#include "Chunk.hpp"

#include "../World.hpp"
#include "../voxel_engine.hpp"
#include "../terrain_gen.hpp"

Chunk::Chunk(World* world, glm::ivec3 position)
    : world(world),
      app(world->getApp()),
      mesh(nullptr),
      m_model(getModelMatrix()),
      is_empty(true),
      position(position),
        voxels_chunk(nullptr),
        center((glm::vec3(position) + glm::vec3(0.5f)) * static_cast<float>(CHUNK_SIZE))
{
    //buildMesh();
}

void Chunk::buildMesh() {
    mesh = new ChunkMesh(this);
}

glm::mat4 Chunk::getModelMatrix() const {
    return glm::translate(glm::mat4(1.0f), glm::vec3(position) * static_cast<float>(CHUNK_SIZE));
}

void Chunk::setUniform() {
    if (mesh) {
        GLuint program = mesh->program;
        app->shaderProgram->setMatrixUniform(program, "m_model", m_model);
    } else {
        std::cout << "Chunk class No mesh" << std::endl;
    }
}

void Chunk::setUniform_with_program(GLuint program) {
    if (mesh) {
        app->shaderProgram->setMatrixUniform(program, "m_model", m_model);
    } else {
        std::cout << "Chunk class No mesh" << std::endl;
    }
}

void Chunk::render() {
    // 好像不用frustum性能更好
    //bool is_on_frustum = app->player->frustum->isOnFrustum(this);
    float dist_to_player = glm::length(glm::vec3(app->player->get_position().x - center.x, app->player->get_position().z - center.z,0.0f));

    if (!is_empty && mesh && dist_to_player < RENDER_RANGE * CHUNK_SIZE) {
            GLuint program = mesh->program;
            app->shaderProgram->use(program);
            this->setUniform();
            mesh->render();
    }
}

void Chunk::render_with_shader_program(GLuint program) {
    // 好像不用frustum性能更好
    //bool is_on_frustum = app->player->frustum->isOnFrustum(this);
    float dist_to_player = glm::length(glm::vec3(app->player->get_position().x - center.x, app->player->get_position().z - center.z,0.0f));

    if (!is_empty && mesh && dist_to_player < RENDER_RANGE * CHUNK_SIZE) {
        //GLuint program = mesh->program;
        app->shaderProgram->use(program);
        this->setUniform_with_program(program);
        mesh->render();
    }
}


std::array<float, CHUNK_VOL> Chunk::buildVoxels() {
    std::array<float, CHUNK_VOL> voxels;
    voxels.fill(0);

    // Get world-space chunk coordinates
    int cx = position.x * CHUNK_SIZE;
    int cy = position.y * CHUNK_SIZE;
    int cz = position.z * CHUNK_SIZE;

    // Fill the chunk based on some noise function (simplex noise)
    for (int x = 0; x < CHUNK_SIZE; ++x) {
        int wx = x + cx;
        for (int z = 0; z < CHUNK_SIZE; ++z) {
            int wz = z + cz;

            int worldHeight = static_cast<int>(glm::simplex(glm::vec2(wx, wz) * 0.01f) * 32 + 32);
            int localHeight = std::min(worldHeight - cy, CHUNK_SIZE);

            for (int y = 0; y < localHeight; ++y) {
                int wy = y + cy;
                voxels[x + CHUNK_SIZE * z + CHUNK_AREA * y] = wy + 1;  // Voxel value
            }
        }
    }
    for (auto & voxel : voxels) {
        if (voxel != 0) {
            is_empty = false;
            break;
        }
    }
    return voxels;
}

void Chunk::generate_terrain(std::array<float, CHUNK_VOL> &voxels, int cx, int cy, int cz) {
    for (int x = 0; x < CHUNK_SIZE; ++x) {
        int wx = x + cx;
        for (int z = 0; z < CHUNK_SIZE; ++z) {
            int wz = z + cz;
            int worldHeight = getHeight(wx, wz);  // Replace with the correct terrain height function
            int localHeight = std::min(worldHeight - cy, CHUNK_SIZE);

            for (int y = 0; y < localHeight; ++y) {
                int wy = y + cy;
                setVoxelID(voxels, x, y, z, wx, wy, wz, worldHeight);  // Call your voxel ID setting function
            }
        }
    }
}

std::array<float, CHUNK_VOL> Chunk::buildVoxels_new() {
    std::array<float, CHUNK_VOL> voxels = {0};  // Initialize array with zeros

    int cx = static_cast<int>(position.x) * CHUNK_SIZE;
    int cy = static_cast<int>(position.y) * CHUNK_SIZE;
    int cz = static_cast<int>(position.z) * CHUNK_SIZE;

    generate_terrain(voxels, cx, cy, cz);

    // Check if the voxel array has any non-zero entries
    is_empty = std::all_of(voxels.begin(), voxels.end(), [](float v) { return v == 0; });
    return voxels;
}

void Chunk::check_empty() {
    is_empty = std::all_of(voxels_chunk->begin(), voxels_chunk->end(), [](float v) { return v == 0; });
}

