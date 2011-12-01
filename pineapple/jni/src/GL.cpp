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

char usefulchars[93]=("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!\"$%&*(){}[]:;@'~#?/\\<>,.+-_+|");

namespace Pineapple {

	GL::GL() {
		width_ = 1;
		height_ = 1;
		for(int i=0;i<93;i++) letterIdxs_[usefulchars[i]] = i;
		memset(fontTextures_, 0, NUMFONTS * sizeof(GLTexture *));
	}

	GL::GL(int w, int h) {
		initializeGL(w, h);
		for(int i=0;i<93;i++) letterIdxs_[usefulchars[i]] = i;
		memset(fontTextures_, 0, NUMFONTS * sizeof(GLTexture *));
	}

	GL::~GL() {
		// TODO Auto-generated destructor stub
	}

	void GL::initializeGL(int w, int h) {
		width_ = w;
		height_ = h;

		glViewport(0, 0, w, h);

	}

	void GL::loadFont(FONTS font) {
		GLTextureParams parms;
		parms.format = GL_LUMINANCE;
		parms.width = 256;
		parms.height = 256;
		if(!fontTextures_[font])
			fontTextures_[font] = new GLTexture(parms, GetFontData(font));
	}


	void GL::createTexture(const std::string &name, GLTextureParams &parms, unsigned char *data) {
		textures_[name] = new GLTexture(parms, data);
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
