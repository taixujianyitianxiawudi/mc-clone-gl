#include "ShadowFramebuffer.hpp"

ShadowFramebuffer::ShadowFramebuffer(int width, int height) {
    // Generate depth texture
    glGenFramebuffers(1, &depthFBO);

    depthTexture = Textures::getDepthTexture(glm::vec2(width, height));

    glBindFramebuffer(GL_FRAMEBUFFER, depthFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture, 0);

    // We only need the depth buffer
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cerr << "Error: Framebuffer not complete!" << std::endl;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);  // Unbind the framebuffer
}

void ShadowFramebuffer::bind() {
    glBindFramebuffer(GL_FRAMEBUFFER, depthFBO);
}

void ShadowFramebuffer::unbind() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

GLuint ShadowFramebuffer::getDepthTexture() {
    return depthTexture;
}

ShadowFramebuffer::~ShadowFramebuffer() {
    glDeleteFramebuffers(1, &depthFBO);
    glDeleteTextures(1, &depthTexture);
}