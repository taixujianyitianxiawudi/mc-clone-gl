#version 330 core

layout (location = 0) in vec3 in_position;  // Vertex position
layout (location = 1) in vec2 in_uv;        // UV coordinates
layout (location = 2) in float voxel_id;
layout (location = 3) in float face_id;

uniform mat4 m_proj;   // Projection matrix
uniform mat4 m_view;   // View matrix
uniform mat4 m_model;  // Model matrix

out vec2 uv;  // Pass UV coordinates to fragment shader
out float voxel_id_out;
out float face_id_out;

const float ao_values[4] = float[4](0.1, 0.25, 0.5, 1.0);

float face_shading[6] = float[6](
1.0, 0.5,  // top bottom
0.5, 0.8,  // right left
0.5, 0.8   // front back
);

const vec2 uv_coords[4] = vec2[4](
vec2(0, 0), vec2(0, 1),
vec2(1, 0), vec2(1, 1)
);

const int uv_indices[24] = int[24](
1, 0, 2, 1, 2, 3,  // tex coords indices for vertices of an even face
3, 0, 2, 3, 1, 0,  // odd face
3, 1, 0, 3, 0, 2,  // even flipped face
1, 2, 3, 1, 0, 2   // odd flipped face
);

out float shading;

void main()
{
    // Transform the vertex position by model, view, and projection matrices
    // print face
    int uv_index = gl_VertexID % 6  + (int(face_id) & 1) * 6;

    shading = face_shading[int(face_id)];
    gl_Position = m_proj * m_view * m_model * vec4(in_position, 1.0);

    // Pass the UV coordinates to the fragment shader
    uv = in_uv;

    voxel_id_out = voxel_id;
    face_id_out = face_id;
}