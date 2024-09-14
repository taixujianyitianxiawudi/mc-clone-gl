//
// Created by zzk on 2024/9/5.
//

#include "Scene.hpp"
#include "voxel_engine.hpp"

// Constructor
Scene::Scene(VoxelEngine* app)
    : app(app)
    //quad(app),
     {
    world = new World(app);
    water = new Water(app);
    voxel_marker = std::make_unique<VoxelMarker>(world->voxel_handler);
    light = new Light(app, glm::vec3(0.0f, 40.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));

}

// Update function (currently empty)
void Scene::update() {
    world->update();
    voxel_marker->update();
    // Update the scene here if needed
}

// Render function to render the quad mesh
void Scene::render() {
    glEnable(GL_CULL_FACE);
    world->render();

    glDisable(GL_CULL_FACE);
    glUseProgram(app->shaderProgram->waterProgram);
    water->render();
    glUseProgram(app->shaderProgram->voxel_marker_program);
    voxel_marker->render();
}

void Scene::render_new() {
    // First pass: Render to the shadow map from the light's perspective
    glEnable(GL_CULL_FACE);
    app->shaderProgram->renderShadowMap();  // This handles shadow map rendering

    // Reset the viewport for the second pass (rendering the scene)
    glViewport(0, 0, WIN_RES.x, WIN_RES.y);  // Set viewport to window size
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // Clear the color and depth buffers for the second pass

    // Second pass: Render the scene from the camera's perspective using shadows
    GLuint program_chunk = app->shaderProgram->chunkProgram;
    glUseProgram(program_chunk);

    // Set the light space matrix uniform (for shadow calculations)
    glm::mat4 lightSpaceMatrix = app->scene->light->getLightSpaceMatrix();
    glUniformMatrix4fv(glGetUniformLocation(program_chunk, "lightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));

    // Bind the shadow map texture (texture unit 1)
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, app->shaderProgram->shadowFramebuffer->getDepthTexture());
    glUniform1i(glGetUniformLocation(program_chunk, "shadowMap"), 1);  // Tell the shader that the shadow map is bound to texture unit 1

    // Render the scene using the chunk program (with shadows applied)
    world->render_with_shader_program(program_chunk);

    // Optionally render water and other elements without shadows
    glDisable(GL_CULL_FACE);

    // Use the water program and render water
    glUseProgram(app->shaderProgram->waterProgram);
    water->render();

    // Render the voxel marker
    glUseProgram(app->shaderProgram->voxel_marker_program);
    voxel_marker->render();
}