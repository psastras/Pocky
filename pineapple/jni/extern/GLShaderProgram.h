#ifndef GLSHADERPROGRAM_H
#define GLSHADERPROGRAM_H
#include "Common.h"
#include "Float3.h"
#include <string>
#include <unordered_map>
#include <vector>

class VSML;
class GLShaderProgram
{
public:
    GLShaderProgram();
    ~GLShaderProgram();

    void loadShaderFromSource(GLenum type, std::string source);
    void loadShaderFromData(GLenum type, unsigned char *data, size_t size);
    bool link();
    void bind() { glUseProgram(programId_); }
    void bind(VSML *instance);
    void release() { glUseProgram(0); }
    void vsml(VSML *instance);
    inline GLint getUniformLocation(const char *name) {
		if(uniforms_.find(name) == uniforms_.end()) {
			uniforms_[name] = glGetUniformLocation(programId_, name);
		}
		return uniforms_[name];
    }

    inline GLint getAttributeLocation(const char *name) {
	if(attributes_.find(name) == attributes_.end()) {
	    attributes_[name] = glGetAttribLocation(programId_, name);
	}
	return attributes_[name];
    }

    inline void setUniformValue(const char *name, bool value) {
	glUniform1i(getUniformLocation(name), value);
    }

    inline void setUniformValue(const char *name, float *mat4x4) {
	 glUniformMatrix4fv(getUniformLocation(name), 1, false, mat4x4);
    }

    inline void setUniformValue(const char *name, float2 *vals, int n) {
	glUniform2fv(getUniformLocation(name), n, &vals->x);
    }

    inline void setUniformValue(const char *name, float *vals, int n) {
	glUniform1fv(getUniformLocation(name), n, vals);
    }

    inline void setUniformValue(const char *name, float2 val) {
    	glUniform2fv(getUniformLocation(name), 1, &val.x);
    }

    inline void setUniformValue(const char *name, Float3 val) {
    	glUniform3fv(getUniformLocation(name), 1, &val.x);
    }

    inline void setUniformValue(const char *name, Float3 *vals, int n) {
        glUniform3fv(getUniformLocation(name), n,&vals[0].x);
    }

    inline void setUniformValue(const char *name, Float4 val) {
        	glUniform4fv(getUniformLocation(name), 1, &val.x);
    }

    inline void setUniformValue(const char *name, float val) {
    	glUniform1f(getUniformLocation(name), val);
    }

    inline void setUniformValue(const char *name, int val){
    	glUniform1i(getUniformLocation(name), val);
    }

	GLuint id() { return programId_; }

protected:

    std::unordered_map <const char *, GLint> uniforms_;
    std::unordered_map <const char *, GLint> attributes_;
    std::vector<GLuint> shaders_;
    GLuint programId_;

};

#endif // GLSHADERPROGRAM_H
