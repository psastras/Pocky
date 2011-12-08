/*
 * PineappleGLSurface.cpp
 *
 *  Created on: Nov 28, 2011
 *      Author: psastras
 */

#include "../extern/GL.h"
#include "../extern/Engine.h"
#ifndef _DESKTOP
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#else
#define GL_GLEXT_PROTOTYPES
#include <QFile>
#include <fstream>
#include <GL/gl.h>
#include <GL/glext.h>
#include <sstream>
#include <iostream>
using namespace std;
#include <qgl.h>
#endif
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
		GL::instance()->createPrimitive("quad", new GLQuad(Float3(10, 6, 10), Float3(w/2,h/2,1.f), Float3(w, h, 1.f)));
		GL::instance()->createShader("text", "assets/shaders/text.glsl");
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

	float2 GL::unproject(const Float3& pos)
	{
		float *p = VSML::instance()->get(VSML::PROJECTION);
		 GLfloat _p[] = {p[0]*pos.x +p[4]*pos.y +p[8]*pos.z + p[12],
		p[1]*pos.x +p[5]*pos.y +p[9]*pos.z + p[13],
		p[2]*pos.x +p[6]*pos.y +p[10]*pos.z+ p[14],
		p[3]*pos.x +p[7]*pos.y +p[11]*pos.z+ p[15]};

		return float2((_p[0]/_p[3]+1.f)*0.5f*width_, (1.f - _p[1]/_p[3])*height_*0.5f);
	}

	Float3 GL::project(const float2 &sspos, const float z)
	{
		float *p = VSML::instance()->get(VSML::PROJECTION);

		const int w = width_;
		const int h = height_;
		const float u = sspos.x;
		const float v = sspos.y;

		// begin lulz - psastras
		// see http://www.wolframalpha.com/input/?i=Solve{u%3D%28%28P0*x%2BP4*y-z*P8%2BP12%29%2F%28P3*x%2BP7*y-z*P11%2BP15%29%2B1%29*0.5*w%2C+v%3D%281.0-%28P1*x%2BP5*y-z*P9%2BP13%29%2F%28P3*x%2BP7*y-z*P11%2BP15%29%29*0.5*h}+for+x+and+y
		float x=-((h*p[5]-h*p[7]+2*p[7]*v)*(-2*p[11]*u*z+p[11]*w*z-p[12]*
				w+2*p[15]*u-p[15]*w+p[8]*w*z)-(-p[4]*w+2*p[7]*u-p[7]*w)*
				(h*p[11]*z+h*p[13]-h*p[15]-h*p[9]*z-2*p[11]*v*z+2*p[15]*v))/
				((h*p[5]-h*p[7]+2*p[7]*v)*(-p[0]*w+2*p[3]*u-p[3]*w)-
						(h*p[1]-h*p[3]+2*p[3]*v)*(-p[4]*w+2*p[7]*u-p[7]*w));
		float y=-(-h*p[0]*p[11]*w*z-h*p[0]*p[13]*w+h*p[0]*p[15]*w+h*p[0]*p[9]
		        *w*z+2*h*p[1]*p[11]*u*z-h*p[1]*p[11]*w*z+h*p[1]*p[12]*w-2*h*
		        p[1]*p[15]*u+h*p[1]*p[15]*w-h*p[1]*p[8]*w*z-h*p[12]*p[3]*w+2*
		        h*p[13]*p[3]*u-h*p[13]*p[3]*w+h*p[3]*p[8]*w*z-2*h*p[3]*p[9]*u*
		        z+h*p[3]*p[9]*w*z+2*p[0]*p[11]*v*w*z-2*p[0]*p[15]*v*w+2*p[12]*
		        p[3]*v*w-2*p[3]*p[8]*v*w*z)/(-h*p[0]*p[5]*w+h*p[0]*p[7]*w+h*
		        		p[1]*p[4]*w-2*h*p[1]*p[7]*u+h*p[1]*p[7]*w-h*p[3]*p[4]*w+
		        		2*h*p[3]*p[5]*u-h*p[3]*p[5]*w-2*p[0]*p[7]*v*w+2*p[3]*p[4]*v*w);
		return Float3(x, y, z);
	}

	void GL::renderText(const std::string &text, const Float3 &pos, FONTS font)
	{
		if(!fontTextures_[font]) this->loadFont(font);
		FONTTABLE *tbl = GetFontTable(font);
		float scale = 0.75f;
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
		float posx = pos.x; float posy = pos.y;
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
				posx = pos.x;
				posy += lineheight + 5.f;
				continue;
			}
			float2 scale0 = {dx+0.003f,dy+0.003f};
			float2 offset0 = {cur->fx-0.003f,cur->fy-0.003f};
			VSML::instance()->pushMatrix(VSML::MODELVIEW);
			VSML::instance()->translate(posx, height_ - lineheight - posy, pos.z);
			VSML::instance()->scale(dx*scale, dy*scale, 1.f);

			GL::instance()->shader("text")->vsml(VSML::instance());
			GL::instance()->shader("text")->setUniformValue("texScale", scale0);
			GL::instance()->shader("text")->setUniformValue("texOffset", offset0);
			GL::instance()->primitive("quad")->draw("text");
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

	void GL::releaseShader(const std::string &name) {
		delete shaders_[name];
		shaders_.erase(name);
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
