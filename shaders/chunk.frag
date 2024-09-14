#version 330 core

in vec2 uv;  // Interpolated UV coordinates from the vertex shader

//uniform sampler2D texture_diffuse;  // Texture sampler for diffuse texture
uniform sampler2DArray u_texture_array_0;
uniform float water_line;
uniform vec3 bg_color;
uniform sampler2D shadowMap;    // Shadow map (depth texture)

out vec4 frag_color;  // Final output color

in float shading;
const vec3 gamma = vec3(2.2);
const vec3 inv_gamma = 1 / gamma;

in float voxel_id_out;
in float face_id_out;
in vec3 frag_world_pos;

void main()
{
    vec2 face_uv = uv;
    face_uv.x = 3 - (uv.x / 3.0 - min(face_id_out, 2) / 3.0);

    vec3 tex_col = texture(u_texture_array_0, vec3(face_uv, voxel_id_out)).rgb;

    vec3 tex_color_rgb = tex_col.rgb;

    tex_color_rgb = pow(tex_color_rgb, gamma);
    tex_color_rgb *= shading;

    // underwater effect
    if (frag_world_pos.y < water_line) {
        tex_color_rgb *= vec3(0.0, 0.3, 1.0);
    }

    //fog
    float fog_dist = gl_FragCoord.z / gl_FragCoord.w;
    tex_color_rgb = mix(tex_color_rgb, bg_color, (1.0 - exp2(-0.00002 * fog_dist * fog_dist)));


    tex_color_rgb = pow(tex_color_rgb, inv_gamma);
    // Output the sampled texture color
    frag_color = vec4(tex_color_rgb, 1);
}