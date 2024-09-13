#version 330 core

layout (location = 0) in vec3 in_position;  // Vertex position
layout (location = 1) in vec2 in_uv;        // UV coordinates
layout (location = 2) in int voxel_id;

uniform mat4 m_proj;   // Projection matrix
uniform mat4 m_view;   // View matrix
uniform mat4 m_model;  // Model matrix

out vec2 frag_uv;  // Pass UV coordinates to fragment shader

void main()
{
    // Transform the vertex position by model, view, and projection matrices
    gl_Position = m_proj * m_view * m_model * vec4(in_position, 1.0);

    // Pass the UV coordinates to the fragment shader
    frag_uv = in_uv;
}