/*
 * PineappleGLSurface.cpp
 *
 *  Created on: Nov 28, 2011
 *      Author: psastras
 */

#include "../extern/GL.h"
#include "../extern/Engine.h"
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>


namespace Pineapple {

	GL::GL() {
		width_ = 1;
		height_ = 1;
	}

	GL::GL(int w, int h) {
		initializeGL(w, h);
	}

	GL::~GL() {
		// TODO Auto-generated destructor stub
	}

	void GL::initializeGL(int w, int h) {
		width_ = w;
		height_ = h;

		glViewport(0, 0, w, h);

	}

	void GL::createShader(const std::string &name, const char *filename) {
		shaders_[name] = new GLShaderProgram();
		size_t size;
		unsigned char *data = Pineapple::Engine::instance()->readResourceFromAPK(filename, size);
		shaders_[name]->loadShaderFromData(GL_FRAGMENT_SHADER, data, size);
		shaders_[name]->loadShaderFromData(GL_VERTEX_SHADER, data, size);
		shaders_[name]->link();
		delete[] data;
	}

	void GL::ortho() {
		VSML::instance()->loadIdentity(VSML::PROJECTION);
		VSML::instance()->ortho(0.f,(float)width_,(float)height_,0.f);
		VSML::instance()->loadIdentity(VSML::MODELVIEW);
	}

	void GL::perspective(float fov, float near, float far) {
		VSML::instance()->perspective(fov, width_ / (float)height_, near, far);
		VSML::instance()->loadIdentity(VSML::MODELVIEW);
	}

}
