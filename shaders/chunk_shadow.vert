#version 330 core

layout (location = 0) in vec3 in_position;  // Vertex position
layout (location = 1) in vec2 in_uv;        // UV coordinates
layout (location = 2) in float voxel_id;
layout (location = 3) in float face_id;

uniform mat4 m_proj;   // Projection matrix
uniform mat4 m_view;   // View matrix
uniform mat4 m_model;  // Model matrix
uniform mat4 lightSpaceMatrix;

out vec2 uv;  // Pass UV coordinates to fragment shader
out float voxel_id_out;
out float face_id_out;
out vec3 frag_world_pos;
out vec4 FragPosLightSpace;
out vec3 FragPos;

const float ao_values[4] = float[4](0.1, 0.25, 0.5, 1.0);

float face_shading[6] = float[6](
1.0, 0.5,  // top bottom
0.8, 0.8,  // right left
0.8, 0.8   // front back
);

out float shading;

void main()
{
    shading = face_shading[int(face_id)];
    gl_Position = m_proj * m_view * m_model * vec4(in_position, 1.0);

    FragPos = vec3(m_model * vec4(in_position, 1.0));
    FragPosLightSpace = lightSpaceMatrix * vec4(FragPos, 1.0);

    frag_world_pos = (m_model * vec4(in_position, 1.0)).xyz;
    // Pass the UV coordinates to the fragment shader
    uv = in_uv;

    voxel_id_out = voxel_id;
    face_id_out = face_id;
}