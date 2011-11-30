#ifndef GLFRAMEBUFFEROBJECT_H
#define GLFRAMEBUFFEROBJECT_H

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

struct GLFramebufferObjectParams {
    int width, height;
    bool hasDepth;
    GLenum format, type;
};

class GLFramebufferObject
{
public:
    GLFramebufferObject(GLFramebufferObjectParams params);
    ~GLFramebufferObject();

    const GLuint *color_attachments() { return color_; }
    GLuint id() { return id_; }
    GLFramebufferObjectParams &params() { return params_;  }
    void bind();
    inline int width() { return params_.width; }
    inline int height() { return params_.height; }

    void bindsurface(int idx);
    void unbindsurface() { glBindTexture(params_.type, 0); }
    void release();

    void resize(int width, int height);

    GLuint* texture();
    GLuint depth();

    void checkStatus();

protected:


    void allocFramebuffer(GLFramebufferObjectParams &params);

    GLuint depth_, *color_, id_;
    GLFramebufferObjectParams params_;
};

#endif // GLFRAMEBUFFEROBJECT_H
