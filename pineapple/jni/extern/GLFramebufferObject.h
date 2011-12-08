#ifndef GLFRAMEBUFFEROBJECT_H
#define GLFRAMEBUFFEROBJECT_H

#include "Common.h"

#ifndef _DESKTOP
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#else
#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glext.h>
#include <qgl.h>
#endif

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

    void releaseFramebuffer();
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
