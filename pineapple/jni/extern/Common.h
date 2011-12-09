#ifndef COMMON_H_
#define COMMON_H_
//#define _DESKTOP

#ifndef _DESKTOP
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#else
#define GL_GLEXT_PROTOTYPES 1
#include <GL/gl.h>
GLAPI void APIENTRY glBlendEquationSeparate (GLenum, GLenum);
GLAPI void APIENTRY glDrawBuffers (GLsizei, const GLenum *);
GLAPI void APIENTRY glStencilOpSeparate (GLenum, GLenum, GLenum, GLenum);
GLAPI void APIENTRY glStencilFuncSeparate (GLenum, GLenum, GLint, GLuint);
GLAPI void APIENTRY glStencilMaskSeparate (GLenum, GLuint);
GLAPI void APIENTRY glAttachShader (GLuint, GLuint);
GLAPI void APIENTRY glBindAttribLocation (GLuint, GLuint, const GLchar *);
GLAPI void APIENTRY glCompileShader (GLuint);
GLAPI GLuint APIENTRY glCreateProgram (void);
GLAPI GLuint APIENTRY glCreateShader (GLenum);
GLAPI void APIENTRY glDeleteProgram (GLuint);
GLAPI void APIENTRY glDeleteShader (GLuint);
GLAPI void APIENTRY glDetachShader (GLuint, GLuint);
GLAPI void APIENTRY glDisableVertexAttribArray (GLuint);
GLAPI void APIENTRY glEnableVertexAttribArray (GLuint);
GLAPI void APIENTRY glGetActiveAttrib (GLuint, GLuint, GLsizei, GLsizei *, GLint *, GLenum *, GLchar *);
GLAPI void APIENTRY glGetActiveUniform (GLuint, GLuint, GLsizei, GLsizei *, GLint *, GLenum *, GLchar *);
GLAPI void APIENTRY glGetAttachedShaders (GLuint, GLsizei, GLsizei *, GLuint *);
GLAPI GLint APIENTRY glGetAttribLocation (GLuint, const GLchar *);
GLAPI void APIENTRY glGetProgramiv (GLuint, GLenum, GLint *);
GLAPI void APIENTRY glGetProgramInfoLog (GLuint, GLsizei, GLsizei *, GLchar *);
GLAPI void APIENTRY glGetShaderiv (GLuint, GLenum, GLint *);
GLAPI void APIENTRY glGetShaderInfoLog (GLuint, GLsizei, GLsizei *, GLchar *);
GLAPI void APIENTRY glGetShaderSource (GLuint, GLsizei, GLsizei *, GLchar *);
GLAPI GLint APIENTRY glGetUniformLocation (GLuint, const GLchar *);
GLAPI void APIENTRY glGetUniformfv (GLuint, GLint, GLfloat *);
GLAPI void APIENTRY glGetUniformiv (GLuint, GLint, GLint *);
GLAPI void APIENTRY glGetVertexAttribdv (GLuint, GLenum, GLdouble *);
GLAPI void APIENTRY glGetVertexAttribfv (GLuint, GLenum, GLfloat *);
GLAPI void APIENTRY glGetVertexAttribiv (GLuint, GLenum, GLint *);
GLAPI void APIENTRY glGetVertexAttribPointerv (GLuint, GLenum, GLvoid* *);
GLAPI GLboolean APIENTRY glIsProgram (GLuint);
GLAPI GLboolean APIENTRY glIsShader (GLuint);
GLAPI void APIENTRY glLinkProgram (GLuint);
GLAPI void APIENTRY glShaderSource (GLuint, GLsizei, const GLchar* *, const GLint *);
GLAPI void APIENTRY glUseProgram (GLuint);
GLAPI void APIENTRY glUniform1f (GLint, GLfloat);
GLAPI void APIENTRY glUniform2f (GLint, GLfloat, GLfloat);
GLAPI void APIENTRY glUniform3f (GLint, GLfloat, GLfloat, GLfloat);
GLAPI void APIENTRY glUniform4f (GLint, GLfloat, GLfloat, GLfloat, GLfloat);
GLAPI void APIENTRY glUniform1i (GLint, GLint);
GLAPI void APIENTRY glUniform2i (GLint, GLint, GLint);
GLAPI void APIENTRY glUniform3i (GLint, GLint, GLint, GLint);
GLAPI void APIENTRY glUniform4i (GLint, GLint, GLint, GLint, GLint);
GLAPI void APIENTRY glUniform1fv (GLint, GLsizei, const GLfloat *);
GLAPI void APIENTRY glUniform2fv (GLint, GLsizei, const GLfloat *);
GLAPI void APIENTRY glUniform3fv (GLint, GLsizei, const GLfloat *);
GLAPI void APIENTRY glUniform4fv (GLint, GLsizei, const GLfloat *);
GLAPI void APIENTRY glUniform1iv (GLint, GLsizei, const GLint *);
GLAPI void APIENTRY glUniform2iv (GLint, GLsizei, const GLint *);
GLAPI void APIENTRY glUniform3iv (GLint, GLsizei, const GLint *);
GLAPI void APIENTRY glUniform4iv (GLint, GLsizei, const GLint *);
GLAPI void APIENTRY glUniformMatrix2fv (GLint, GLsizei, GLboolean, const GLfloat *);
GLAPI void APIENTRY glUniformMatrix3fv (GLint, GLsizei, GLboolean, const GLfloat *);
GLAPI void APIENTRY glUniformMatrix4fv (GLint, GLsizei, GLboolean, const GLfloat *);
GLAPI void APIENTRY glValidateProgram (GLuint);
GLAPI void APIENTRY glVertexAttrib1d (GLuint, GLdouble);
GLAPI void APIENTRY glVertexAttrib1dv (GLuint, const GLdouble *);
GLAPI void APIENTRY glVertexAttrib1f (GLuint, GLfloat);
GLAPI void APIENTRY glVertexAttrib1fv (GLuint, const GLfloat *);
GLAPI void APIENTRY glVertexAttrib1s (GLuint, GLshort);
GLAPI void APIENTRY glVertexAttrib1sv (GLuint, const GLshort *);
GLAPI void APIENTRY glVertexAttrib2d (GLuint, GLdouble, GLdouble);
GLAPI void APIENTRY glVertexAttrib2dv (GLuint, const GLdouble *);
GLAPI void APIENTRY glVertexAttrib2f (GLuint, GLfloat, GLfloat);
GLAPI void APIENTRY glVertexAttrib2fv (GLuint, const GLfloat *);
GLAPI void APIENTRY glVertexAttrib2s (GLuint, GLshort, GLshort);
GLAPI void APIENTRY glVertexAttrib2sv (GLuint, const GLshort *);
GLAPI void APIENTRY glVertexAttrib3d (GLuint, GLdouble, GLdouble, GLdouble);
GLAPI void APIENTRY glVertexAttrib3dv (GLuint, const GLdouble *);
GLAPI void APIENTRY glVertexAttrib3f (GLuint, GLfloat, GLfloat, GLfloat);
GLAPI void APIENTRY glVertexAttrib3fv (GLuint, const GLfloat *);
GLAPI void APIENTRY glVertexAttrib3s (GLuint, GLshort, GLshort, GLshort);
GLAPI void APIENTRY glVertexAttrib3sv (GLuint, const GLshort *);
GLAPI void APIENTRY glVertexAttrib4Nbv (GLuint, const GLbyte *);
GLAPI void APIENTRY glVertexAttrib4Niv (GLuint, const GLint *);
GLAPI void APIENTRY glVertexAttrib4Nsv (GLuint, const GLshort *);
GLAPI void APIENTRY glVertexAttrib4Nub (GLuint, GLubyte, GLubyte, GLubyte, GLubyte);
GLAPI void APIENTRY glVertexAttrib4Nubv (GLuint, const GLubyte *);
GLAPI void APIENTRY glVertexAttrib4Nuiv (GLuint, const GLuint *);
GLAPI void APIENTRY glVertexAttrib4Nusv (GLuint, const GLushort *);
GLAPI void APIENTRY glVertexAttrib4bv (GLuint, const GLbyte *);
GLAPI void APIENTRY glVertexAttrib4d (GLuint, GLdouble, GLdouble, GLdouble, GLdouble);
GLAPI void APIENTRY glVertexAttrib4dv (GLuint, const GLdouble *);
GLAPI void APIENTRY glVertexAttrib4f (GLuint, GLfloat, GLfloat, GLfloat, GLfloat);
GLAPI void APIENTRY glVertexAttrib4fv (GLuint, const GLfloat *);
GLAPI void APIENTRY glVertexAttrib4iv (GLuint, const GLint *);
GLAPI void APIENTRY glVertexAttrib4s (GLuint, GLshort, GLshort, GLshort, GLshort);
GLAPI void APIENTRY glVertexAttrib4sv (GLuint, const GLshort *);
GLAPI void APIENTRY glVertexAttrib4ubv (GLuint, const GLubyte *);
GLAPI void APIENTRY glVertexAttrib4uiv (GLuint, const GLuint *);
GLAPI void APIENTRY glVertexAttrib4usv (GLuint, const GLushort *);
GLAPI void APIENTRY glVertexAttribPointer (GLuint, GLint, GLenum, GLboolean, GLsizei, const GLvoid *);
#include <GL/glext.h>
#include <qgl.h>
#endif

#ifndef _DESKTOP
#include <jni.h>
#include <android/log.h>
#define  LOG_TAG    "libpineapplejni"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
#else
#define  LOGI(...)  printf(__VA_ARGS__); printf("\n");fflush(stdout);
#define  LOGE(...)  printf(__VA_ARGS__); printf("\n");fflush(stdout);
#endif

#define  RELEASE(X) \
	if(X) delete X; \
	X = 0;
#define MAX(x, y) (x) > (y) ? (x) : (y)
#define MIN(x, y) (x) < (y) ? (x) : (y)

static int NextPowerOfTwo(int val) {
	val--;
	val = (val >> 1) | val;
	val = (val >> 2) | val;
	val = (val >> 4) | val;
	val = (val >> 8) | val;
	val = (val >> 16) | val;
	val++; // Val is now the next highest power of 2.
	return val;
}

#endif /* COMMON_H_ */
