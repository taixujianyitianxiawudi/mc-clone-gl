#version 330 core

in vec2 frag_uv;  // Interpolated UV coordinates from the vertex shader

uniform sampler2D texture_diffuse;  // Texture sampler for diffuse texture

out vec4 frag_color;  // Final output color

void main()
{
    // Sample the texture using UV coordinates
    vec4 tex_color = texture(texture_diffuse, frag_uv);

    // Output the sampled texture color
    frag_color = tex_color;
}