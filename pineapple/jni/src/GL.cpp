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

/* DO NOT CHANGE THIS ON PAIN OF DEATH */
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
//		glEnable(GL_CULL_FACE);
//		glCullFace(GL_BACK);
//		glFrontFace(GL_CW);
		glDisable(GL_DITHER);
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

	void GL::renderText(const std::string &text, FONTS font)
	{
		if(!fontTextures_[font]) this->loadFont(font);
		FONTTABLE *tbl = GetFontTable(font);
		float scale = 0.5f;
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc (GL_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR);
		VSML::instance()->loadIdentity(VSML::PROJECTION);
		VSML::instance()->ortho(0.f, (float)width_, 0.f, (float)height_);
		VSML::instance()->loadIdentity(VSML::MODELVIEW);
		GL::instance()->shader("text")->bind(VSML::instance());
		glActiveTexture(GL_TEXTURE0);
		fontTextures_[font]->bind();
		GL::instance()->shader("text")->setUniformValue("tex", 0);
		float posx = 5.f; float posy = 5.f;
		int lineheight = 40 * scale; //in pixels - should be calculated in the font generation step
		for(int i=0;i<text.size();i++) {

			int idx = letterIdxs_[text[i]];
			FONTTABLE *cur = &tbl[idx];
			float dy = cur->fy2 - cur->fy;
			float dx = cur->fx2 - cur->fx;
			if(text[i] == ' ')
			{
				posx += dx * width_ *scale + 1;
				continue;
			}

			else if(text[i] == '\n')
			{
				posx = 5.f;
				posy += lineheight + 5.f;
				continue;
			}
			float2 scale0 = {dx+0.003f,dy+0.003f};
			float2 offset0 = {cur->fx-0.003f,cur->fy-0.003f};
			VSML::instance()->pushMatrix(VSML::MODELVIEW);
			VSML::instance()->translate(posx, height_ - lineheight - posy, 0.f);
			VSML::instance()->scale(dx*scale, dy*scale, 1.f);

			GL::instance()->shader("text")->vsml(VSML::instance());
			GL::instance()->shader("text")->setUniformValue("texScale", scale0);
			GL::instance()->shader("text")->setUniformValue("texOffset", offset0);
			GL::instance()->primitive("quad")->draw("texmap");
			VSML::instance()->popMatrix(VSML::MODELVIEW);

			posx += dx * width_ *scale + 1;
		}
		GL::instance()->shader("text")->release();
		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_BLEND);
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

	void GL::ortho(int w, int h) {
		VSML::instance()->loadIdentity(VSML::PROJECTION);
		VSML::instance()->ortho(0.f,(float)w,(float)h,0.f);
		VSML::instance()->loadIdentity(VSML::MODELVIEW);
	}


	void GL::perspective(float fov, float near, float far) {
		VSML::instance()->perspective(fov, width_ / (float)height_, near, far);
		VSML::instance()->loadIdentity(VSML::MODELVIEW);
	}

	void GL::perspective(float fov, float near, float far, int w, int h) {
		VSML::instance()->perspective(fov, w / (float)h, near, far);
		VSML::instance()->loadIdentity(VSML::MODELVIEW);
	}

}
