#ifndef SHADOWFRAMEBUFFER_HPP
#define SHADOWFRAMEBUFFER_HPP

#include "settings.hpp"
#include "Textures.hpp"

class ShadowFramebuffer {
public:
    ShadowFramebuffer(int width, int height);
    ~ShadowFramebuffer();

    void bind();
    void unbind();

    GLuint getDepthTexture();

private:
    GLuint depthFBO;
    GLuint depthTexture;
};

#endif
