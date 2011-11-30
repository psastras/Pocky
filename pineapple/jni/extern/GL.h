/*
 * PineappleGLSurface.h
 *
 *  Created on: Nov 28, 2011
 *      Author: psastras
 */

#ifndef PINEAPPLEGLSURFACE_H_
#define PINEAPPLEGLSURFACE_H_
#include "Common.h"
#include "GLPrimitive.h"
#include "GLShaderProgram.h"
#include "GLFramebufferObject.h"
#include "VSML.h"
#include <unordered_map>
namespace Pineapple {

class GLShader;

struct GLObject {
	GLShader *shader;

};

class GL {
public:
	static void init(int w, int h) {
		if(!s_instance)	s_instance = new GL(w, h);
	}

	static void init() {
			if(!s_instance)	s_instance = new GL();
		}

	static GL *instance() {
		return s_instance;
	}

	virtual ~GL();

	void test();

	void createShader(const std::string &name, const char *filename);
	GLShaderProgram *shader(const std::string &name) { return shaders_[name]; }
	void initializeGL(int w, int h);
	void ortho();
	void perspective(float fov, float near, float far);
protected:
		GL();
		GL(int w, int h);
		int width_, height_;
		static GL *s_instance;
		std::unordered_map<std::string, GLShaderProgram *> shaders_;
		std::unordered_map<std::string, GLPrimitive *> primitives;
	};
}
#endif /* PINEAPPLEGLSURFACE_H_ */
