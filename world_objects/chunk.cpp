#include <set>

#include "Chunk.hpp"

#include "../World.hpp"
#include "../voxel_engine.hpp"

Chunk::Chunk(World* world, glm::vec3 position)
    : world(world),
      app(world->getApp()),
      mesh(nullptr),
      m_model(getModelMatrix()),
      is_empty(true),
      position(position),
        voxels(nullptr)
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

void Chunk::render() {
    if (!is_empty) {
        if (mesh) {
            GLuint program = mesh->program;
            app->shaderProgram->use(program);
            this->setUniform();
            mesh->render();
        }
    }

}


std::array<int, CHUNK_VOL> Chunk::buildVoxels() {
    std::array<int, CHUNK_VOL> voxels;
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

