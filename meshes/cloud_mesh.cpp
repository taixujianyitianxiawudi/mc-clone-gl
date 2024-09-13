//
// Created by zzk on 2024/9/12.
//

#include "cloud_mesh.hpp"

#include <set>

#include "../voxel_engine.hpp"


CloudMesh::CloudMesh(VoxelEngine* app) : app(app), vao(0), vbo(0) {
    // Initialize the shader program for clouds
    program = app->shaderProgram->cloudsProgram;

    // Generate clouds and build the mesh
    generateClouds();
    buildMesh();

    // Generate and bind VAO
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Generate and bind VBO
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, meshData.size() * sizeof(uint16_t), meshData.data(), GL_STATIC_DRAW);

    // Specify layout of vertex attributes
    glEnableVertexAttribArray(0);  // in_position
    glVertexAttribPointer(0, 3, GL_UNSIGNED_SHORT, GL_FALSE, 3 * sizeof(uint16_t), (void*)0);

    glBindVertexArray(0);  // Unbind VAO
}

CloudMesh::~CloudMesh() {
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
}

// Generate cloud data based on noise
void CloudMesh::generateClouds() {
    int width = WORLD_W * CHUNK_SIZE;
    int depth = WORLD_D * CHUNK_SIZE;

    cloudData.resize(WORLD_AREA * CHUNK_SIZE * CHUNK_SIZE, 0);  // Initialize with zeros

    for (int x = 0; x < width; ++x) {
        for (int z = 0; z < depth; ++z) {
            if (noise.noise2(0.13f * x, 0.13f * z) >= 0.2f) {
                cloudData[x + width * z] = 1;  // Mark as cloud if noise value is high enough
            }
        }
    }
}

// Build the mesh for the cloud layer
void CloudMesh::buildMesh() {
    int width = WORLD_W * CHUNK_SIZE;
    int depth = WORLD_D * CHUNK_SIZE;
    int y = CLOUD_HEIGHT;
    std::set<int> visited;
    int index = 0;

    for (int z = 0; z < depth; ++z) {
        for (int x = 0; x < width; ++x) {
            int idx = x + width * z;
            if (!cloudData[idx] || visited.count(idx)) {
                continue;
            }

            // Find continuous quads along x
            int xCount = 1;
            while (x + xCount < width && cloudData[(x + xCount) + width * z] && !visited.count((x + xCount) + width * z)) {
                xCount++;
            }

            // Find continuous quads along z for each x
            std::vector<int> zCountList;
            for (int ix = 0; ix < xCount; ++ix) {
                int zCount = 1;
                while (z + zCount < depth && cloudData[(x + ix) + width * (z + zCount)] && !visited.count((x + ix) + width * (z + zCount))) {
                    zCount++;
                }
                zCountList.push_back(zCount);
            }

            // Use minimum zCount to form a larger quad
            int zCount = *std::min_element(zCountList.begin(), zCountList.end());

            // Mark all quads as visited
            for (int ix = 0; ix < xCount; ++ix) {
                for (int iz = 0; iz < zCount; ++iz) {
                    visited.insert((x + ix) + width * (z + iz));
                }
            }

            // Define vertices for the quad
            glm::ivec3 v0 = glm::ivec3(x, y, z);
            glm::ivec3 v1 = glm::ivec3(x + xCount, y, z + zCount);
            glm::ivec3 v2 = glm::ivec3(x + xCount, y, z);
            glm::ivec3 v3 = glm::ivec3(x, y, z + zCount);

            // Add vertices to mesh data
            std::vector<glm::ivec3> vertices = {v0, v1, v2, v0, v3, v1};
            for (const auto& vertex : vertices) {
                meshData.push_back(vertex.x);
                meshData.push_back(vertex.y);
                meshData.push_back(vertex.z);
            }
        }
    }
}

// Render the cloud mesh
void CloudMesh::render() {
    glUseProgram(program);
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, meshData.size() / 3);
    glBindVertexArray(0);
}