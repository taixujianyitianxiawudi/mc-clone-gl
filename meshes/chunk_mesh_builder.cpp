//
// Created by zzk on 2024/9/7.
//

#include "chunk_mesh_builder.hpp"


// Check if the voxel at the given position is void (empty)
bool isVoid(const std::array<int, 3>& localVoxelPos,
    glm::vec3 worldVoxelPos,
    std::array<std::array<int, CHUNK_VOL>, WORLD_VOL>& worldVoxels) {
    int chunkIndex = getChunkIndex(worldVoxelPos);
    if (chunkIndex == -1) {
        return false;
    }
    if (chunkIndex >= WORLD_VOL) {
        return true;
    }
    std::array<int, CHUNK_VOL>& chunkVoxels = worldVoxels[chunkIndex];
    int x = localVoxelPos[0];
    int y = localVoxelPos[1];
    int z = localVoxelPos[2];

    //if(x < 0 || x >= CHUNK_SIZE || y < 0 || y >= CHUNK_SIZE || z < 0 || z >= CHUNK_SIZE) {
    //    return true;
    //}
    if(x <= 0  || y <= 0 || z <= 0) {
        return true;
    }
    int voxelIndex = x % CHUNK_SIZE + z % CHUNK_SIZE * CHUNK_SIZE + y % CHUNK_SIZE * CHUNK_AREA;

    if (chunkVoxels[voxelIndex]) {
        return false;
    }
    return true;
}

// Helper function to add vertex data into the vertex data array
int addData(std::vector<int>& vertexData, int index, const std::array<std::array<int, 7>, 6>& vertices) {
    for (const auto& vertex : vertices) {
        for (int attr : vertex) {
            vertexData[index] = static_cast<int>(attr);
            index += 1;
        }
    }
    return index;
}

// Build the chunk mesh based on chunk voxels
std::vector<int> buildChunkMesh(
    const std::array<int, CHUNK_VOL>& chunkVoxels,
    int formatSize,
    glm::vec3 chunkPos,
    std::array<std::array<int, CHUNK_VOL>, WORLD_VOL>& worldVoxels) {

    std::vector<int> vertexData(CHUNK_VOL * 18 * formatSize);  // Preallocate enough space
    int index = 0;

    for (int x = 0; x < CHUNK_SIZE; ++x) {
        for (int y = 0; y < CHUNK_SIZE; ++y) {
            for (int z = 0; z < CHUNK_SIZE; ++z) {
                int voxelId = chunkVoxels[x + CHUNK_SIZE * z + CHUNK_AREA * y];

                if (voxelId == 0) {
                    continue;  // Skip empty voxels
                }
                int cx = chunkPos.x;
                int cy = chunkPos.y;
                int cz = chunkPos.z;
                int wx = x + cx * CHUNK_SIZE;
                int wy = y + cy * CHUNK_SIZE;
                int wz = z + cz * CHUNK_SIZE;

                // Define each face of the voxel and check if it's exposed (i.e., adjacent to a void)
                // Each face has 6 vertices (two triangles)

                // Top face
                if (isVoid({x, y + 1, z}, {wx, wy + 1, wz}, worldVoxels)) {
                    // Define vertices for the top face (x, y, z, voxel_id, face_id)
                    auto ao = get_ao({x, y + 1, z}, {wx, wy + 1, wz}, worldVoxels, 'Y');
                    bool flip_id = (ao[1] + ao[3]) > (ao[0] + ao[2]);

                    std::array<int, 7> v0 = {x,     y + 1, z,     voxelId, 0, ao[0], flip_id};
                    std::array<int, 7> v1 = {x + 1, y + 1, z,     voxelId, 0, ao[1], flip_id};
                    std::array<int, 7> v2 = {x + 1, y + 1, z + 1, voxelId, 0, ao[2], flip_id};
                    std::array<int, 7> v3 = {x,     y + 1, z + 1, voxelId, 0, ao[3], flip_id};

                    if (flip_id) {
                        index = addData(vertexData, index, {v1, v0, v3, v1, v3, v2});
                    } else {
                        index = addData(vertexData, index, {v0, v3, v2, v0, v2, v1});
                    }
                }

                // Bottom face
                if (isVoid({x, y - 1, z}, {wx, wy - 1, wz}, worldVoxels)) {
                    auto ao = get_ao({x, y - 1, z}, {wx, wy - 1, wz}, worldVoxels, 'Y');
                    bool flip_id = (ao[1] + ao[3]) > (ao[0] + ao[2]);

                    std::array<int, 7> v0 = {x,     y,     z,     voxelId, 1, ao[0], flip_id};
                    std::array<int, 7> v1 = {x + 1, y,     z,     voxelId, 1, ao[1], flip_id};
                    std::array<int, 7> v2 = {x + 1, y,     z + 1, voxelId, 1, ao[2], flip_id};
                    std::array<int, 7> v3 = {x,     y,     z + 1, voxelId, 1, ao[3], flip_id};
                    if (flip_id) {
                        index = addData(vertexData, index, {v1, v3, v0, v1, v2, v3});
                    } else {
                        index = addData(vertexData, index, {v0, v2, v3, v0, v1, v2});
                    }
                }

                // Right face
                if (isVoid({x + 1, y, z}, {wx + 1, wy, wz}, worldVoxels)) {
                    auto ao = get_ao({x + 1, y, z}, {wx + 1, wy, wz}, worldVoxels, 'X');
                    bool flip_id = (ao[1] + ao[3]) > (ao[0] + ao[2]);
                    std::array<int, 7> v0 = {x + 1, y,     z,     voxelId, 2, ao[0], flip_id};
                    std::array<int, 7> v1 = {x + 1, y + 1, z,     voxelId, 2, ao[1], flip_id};
                    std::array<int, 7> v2 = {x + 1, y + 1, z + 1, voxelId, 2, ao[2], flip_id};
                    std::array<int, 7> v3 = {x + 1, y,     z + 1, voxelId, 2, ao[3], flip_id};

                    if (flip_id) {
                        index = addData(vertexData, index, {v3, v0, v1, v3, v1, v2});
                    } else {
                        index = addData(vertexData, index, {v0, v1, v2, v0, v2, v3});
                    }
                }

                // Left face
                if (isVoid({x - 1, y, z}, {wx - 1, wy, wz}, worldVoxels)) {
                    auto ao = get_ao({x - 1, y, z}, {wx - 1, wy, wz}, worldVoxels, 'X');
                    bool flip_id = (ao[1] + ao[3]) > (ao[0] + ao[2]);

                    std::array<int, 7> v0 = {x,     y,     z,     voxelId, 3, ao[0], flip_id};
                    std::array<int, 7> v1 = {x,     y + 1, z,     voxelId, 3, ao[1], flip_id};
                    std::array<int, 7> v2 = {x,     y + 1, z + 1, voxelId, 3, ao[2], flip_id};
                    std::array<int, 7> v3 = {x,     y,     z + 1, voxelId, 3, ao[3], flip_id};

                    if (flip_id) {
                        index = addData(vertexData, index, {v3, v1, v0, v3, v2, v1});
                    } else {
                        index = addData(vertexData, index, {v0, v2, v1, v0, v3, v2});
                    }
                }

                // Back face
                if (isVoid({x, y, z - 1}, {wx, wy, wz + 1}, worldVoxels)) {
                    auto ao = get_ao({x, y, z - 1}, {wx, wy, wz + 1}, worldVoxels, 'Z');
                    bool flip_id = (ao[1] + ao[3]) > (ao[0] + ao[2]);

                    std::array<int, 7> v0 = {x,     y,     z,     voxelId, 4, ao[0], flip_id};
                    std::array<int, 7> v1 = {x,     y + 1, z,     voxelId, 4, ao[1], flip_id};
                    std::array<int, 7> v2 = {x + 1, y + 1, z,     voxelId, 4, ao[2], flip_id};
                    std::array<int, 7> v3 = {x + 1, y,     z,     voxelId, 4, ao[3], flip_id};

                    if (flip_id) {
                        index = addData(vertexData, index, {v3, v0, v1, v3, v1, v2});
                    } else {
                        index = addData(vertexData, index, {v0, v1, v2, v0, v2, v3});
                    }
                }

                // Front face
                if (isVoid({x, y, z + 1}, {wx, wy, wz + 1}, worldVoxels)) {
                    auto ao = get_ao({x, y, z + 1}, {wx, wy, wz + 1}, worldVoxels, 'Z');
                    bool flip_id = (ao[1] + ao[3]) > (ao[0] + ao[2]);

                    std::array<int, 7> v0 = {x,     y,     z + 1, voxelId, 5, ao[0], flip_id};
                    std::array<int, 7> v1 = {x,     y + 1, z + 1, voxelId, 5, ao[1], flip_id};
                    std::array<int, 7> v2 = {x + 1, y + 1, z + 1, voxelId, 5, ao[2], flip_id};
                    std::array<int, 7> v3 = {x + 1, y,     z + 1, voxelId, 5, ao[3], flip_id};

                    if (flip_id) {
                        index = addData(vertexData, index, {v3, v1, v0, v3, v2, v1});
                    } else {
                        index = addData(vertexData, index, {v0, v2, v1, v0, v3, v2});
                    }
                }
            }
        }
    }

    vertexData.resize(index);  // Trim any unused space
    return vertexData;
}

int getChunkIndex(glm::vec3 world_voxel_pos) {
    int wx = world_voxel_pos.x;
    int wy = world_voxel_pos.y;
    int wz = world_voxel_pos.z;
    int cx = wx / CHUNK_SIZE;
    int cy = wy / CHUNK_SIZE;
    int cz = wz / CHUNK_SIZE;

    if (!(0 <= cx < WORLD_W and 0 <= cy < WORLD_H and 0 <= cz < WORLD_D)) {
        return -1;
    }

    int index = cx + WORLD_W * cz + WORLD_AREA * cy;
    return index;


}
std::array<int, 4> get_ao(const std::array<int, 3>& localVoxelPos, glm::vec3 worldVoxelPos,
                                      std::array<std::array<int, CHUNK_VOL>, WORLD_VOL>& worldVoxels, char plane) {
    int x = localVoxelPos[0], y = localVoxelPos[1], z = localVoxelPos[2];
    int wx = worldVoxelPos.x, wy = worldVoxelPos.y, wz = worldVoxelPos.z;

    int a, b, c, d, e, f, g, h;

    if (plane == 'Y') {
        a = isVoid({x    , y, z - 1}, {wx    , wy, wz - 1}, worldVoxels);
        b = isVoid({x - 1, y, z - 1}, {wx - 1, wy, wz - 1}, worldVoxels);
        c = isVoid({x - 1, y, z    }, {wx - 1, wy, wz    }, worldVoxels);
        d = isVoid({x - 1, y, z + 1}, {wx - 1, wy, wz + 1}, worldVoxels);
        e = isVoid({x    , y, z + 1}, {wx    , wy, wz + 1}, worldVoxels);
        f = isVoid({x + 1, y, z + 1}, {wx + 1, wy, wz + 1}, worldVoxels);
        g = isVoid({x + 1, y, z    }, {wx + 1, wy, wz    }, worldVoxels);
        h = isVoid({x + 1, y, z - 1}, {wx + 1, wy, wz - 1}, worldVoxels);
    }
    else if (plane == 'X') {
        a = isVoid({x, y    , z - 1}, {wx, wy    , wz - 1}, worldVoxels);
        b = isVoid({x, y - 1, z - 1}, {wx, wy - 1, wz - 1}, worldVoxels);
        c = isVoid({x, y - 1, z    }, {wx, wy - 1, wz    }, worldVoxels);
        d = isVoid({x, y - 1, z + 1}, {wx, wy - 1, wz + 1}, worldVoxels);
        e = isVoid({x, y    , z + 1}, {wx, wy    , wz + 1}, worldVoxels);
        f = isVoid({x, y + 1, z + 1}, {wx, wy + 1, wz + 1}, worldVoxels);
        g = isVoid({x, y + 1, z    }, {wx, wy + 1, wz    }, worldVoxels);
        h = isVoid({x, y + 1, z - 1}, {wx, wy + 1, wz - 1}, worldVoxels);
    }
    else {  // Z plane
        a = isVoid({x - 1, y    , z}, {wx - 1, wy    , wz}, worldVoxels);
        b = isVoid({x - 1, y - 1, z}, {wx - 1, wy - 1, wz}, worldVoxels);
        c = isVoid({x    , y - 1, z}, {wx    , wy - 1, wz}, worldVoxels);
        d = isVoid({x + 1, y - 1, z}, {wx + 1, wy - 1, wz}, worldVoxels);
        e = isVoid({x + 1, y    , z}, {wx + 1, wy    , wz}, worldVoxels);
        f = isVoid({x + 1, y + 1, z}, {wx + 1, wy + 1, wz}, worldVoxels);
        g = isVoid({x    , y + 1, z}, {wx    , wy + 1, wz}, worldVoxels);
        h = isVoid({x - 1, y + 1, z}, {wx - 1, wy + 1, wz}, worldVoxels);
    }

    int ao1 = a + b + c;
    int ao2 = g + h + a;
    int ao3 = e + f + g;
    int ao4 = c + d + e;
    return std::array<int, 4>{ao1, ao2, ao3, ao4};
}