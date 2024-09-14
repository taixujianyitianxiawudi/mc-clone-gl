#version 330 core

in vec2 uv;  // Interpolated UV coordinates from the vertex shader

//uniform sampler2D texture_diffuse;  // Texture sampler for diffuse texture
uniform sampler2DArray u_texture_array_0;
uniform float water_line;
uniform vec3 bg_color;
uniform sampler2D shadowMap;    // Shadow map (depth texture)
uniform mat4 lightSpaceMatrix;

out vec4 frag_color;  // Final output color

in float shading;
const vec3 gamma = vec3(2.2);
const vec3 inv_gamma = 1 / gamma;

in float voxel_id_out;
in float face_id_out;
in vec3 frag_world_pos;
in vec4 FragPosLightSpace;

// Function to calculate the shadow factor
float ShadowCalculation(vec4 fragPosLightSpace) {
    // Perform perspective divide to get normalized coordinates
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;  // Transform to [0, 1] range

    // Get the depth value from the shadow map at the fragment's position
    float closestDepth = texture(shadowMap, projCoords.xy).r;
    float currentDepth = projCoords.z;  // Current fragment depth in light space

    // Shadow calculation: if the fragment is farther than the closest depth in the shadow map, it's in shadow
    float bias = 0.005;  // Small bias to prevent shadow acne
    float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;

    // Return shadow factor (1.0 if in shadow, 0.0 otherwise)
    return shadow;
}

void main()
{
    vec3 color = vec3(1.0);


    // Shadow calculation
    float shadow = ShadowCalculation(FragPosLightSpace);
    float diff = 1.0;
    vec3 lighting = (1.0 - shadow) * diff * color;
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
    tex_color_rgb = mix(tex_color_rgb, bg_color, (1.0 - exp2(-0.00001 * fog_dist * fog_dist)));


    tex_color_rgb = pow(tex_color_rgb, inv_gamma);
    // Output the sampled texture color
    frag_color = vec4(lighting, 1);
}