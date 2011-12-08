#ifndef GLPRIMITIVE_H
#define GLPRIMITIVE_H
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
#include "GLShaderProgram.h"
struct GLVertex
{
  Float3 p; Float3 t;
};


class GLPrimitive {
  public:
      ~GLPrimitive();

      virtual void tesselate(Float3 tess, Float3 translate, Float3 scale) = 0; //tesselates and reuploads into vbo
      void draw(const std::string &shadername);
      void draw(GLShaderProgram *program);
      void draw(GLShaderProgram *program, int instances);

      GLuint vertexID() { return vertexId_; }
      GLuint indexID() { return indexId_; }

      const Float3& scale() { return scale_; }
      const Float3& translate() { return scale_; }
  protected:
      GLPrimitive(Float3 &tess, Float3 &translate, Float3 &scale);

      GLuint vertexId_, indexId_;
      GLenum type_;
      GLuint idxCount_;
      GLuint typeCount_;
      int vOffset_, tOffset_;
      GLShaderProgram *shader_;
      Float3 scale_, translate_;
};

class GLQuad : public GLPrimitive {
    public:
	GLQuad(Float3 tess, Float3 translate, Float3 scale, bool flip = false);
	~GLQuad();

	void tesselate(Float3 tess, Float3 translate, Float3 scale);

    protected:
	bool flip_;
};

class GLRect : public GLPrimitive {
public:
    GLRect(Float3 tess, Float3 translate, Float3 scale);
    ~GLRect();

    void tesselate(Float3 tess, Float3 translate, Float3 scale);
};

class GLCircle : public GLPrimitive {
public:
	GLCircle(Float3 tess, Float3 translate, Float3 scale);
    ~GLCircle();

    void tesselate(Float3 tess, Float3 translate, Float3 scale);
};

class GLDisc : public GLPrimitive {
public:
	GLDisc(Float3 tess, Float3 translate, Float3 scale);
    ~GLDisc();

    void tesselate(Float3 tess, Float3 translate, Float3 scale);
};

class GLPlane : public GLPrimitive {
    public:
	GLPlane(Float3 tess, Float3 translate, Float3 scale);
	~GLPlane();

	void tesselate(Float3 tess, Float3 translate, Float3 scale);
};


class GLTriangle : public GLPrimitive {
    public:
	GLTriangle(Float3 tess, Float3 translate, Float3 scale);
	~GLTriangle();

	void tesselate(Float3 tess, Float3 translate, Float3 scale);
};

#endif // GLPRIMITIVE_H
