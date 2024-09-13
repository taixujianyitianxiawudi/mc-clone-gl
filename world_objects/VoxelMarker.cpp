#include "VoxelMarker.hpp"

#include "../voxel_engine.hpp"

VoxelMarker::VoxelMarker(VoxelHandler* voxelHandler)
    : handler(voxelHandler), position(glm::vec3(0)), mModel(getModelMatrix()) {
    mesh = new CubeMesh(handler->world->getApp());  // Assuming CubeMesh constructor takes app as parameter
}

void VoxelMarker::update() {
    if (handler->voxelID) {
        if (handler->interactionMode == 1) {  // If in add mode
            //position = handler->voxelWorldPos + handler->voxelNormal;
            position = handler->voxelWorldPos + handler->voxelNormal;
        } else {  // In remove mode
            position = handler->voxelWorldPos;
        }
    }
}

void VoxelMarker::setUniform() {
    // Assuming you have a way to set uniforms in your CubeMesh shader program
    //mesh->program->setUniform("mode_id", handler->interactionMode);  // mode_id is the interaction mode
    //mesh->program->setUniform("m_model", getModelMatrix());  // m_model is the transformation matrix
    glUseProgram(mesh->program);
    mesh->app->shaderProgram->setIntUniform(mesh->program, "mode_id", handler->interactionMode);
    mesh->app->shaderProgram->setMatrixUniform(mesh->program, "m_model", getModelMatrix());

}

glm::mat4 VoxelMarker::getModelMatrix() {
    return glm::translate(glm::mat4(1.0f), position);
}

void VoxelMarker::render() {
    if (handler->voxelID) {
        setUniform();
        mesh->render();  // Render the cube mesh
    }
}