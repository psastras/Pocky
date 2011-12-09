/*
 * PockyGame.cpp
 *
 *  Created on: Dec 4, 2011
 *      Author: psastras
 */

#include "../include/PockyGame.h"
#include <pineapple/jni/extern/Engine.h>
#include <pineapple/jni/extern/Compile.h>
#include <pineapple/jni/extern/Common.h>
#include <pineapple/jni/extern/GL.h>
#include <pineapple/jni/extern/GLText.h>
#include <pineapple/jni/extern/Audio.h>
#ifndef _DESKTOP
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#else
#define GL_GLEXT_PROTOTYPES
#include <qdebug.h>
#include <GL/gl.h>
#include <GL/glext.h>
#include <qgl.h>
#endif
#include <sstream>
#include <iomanip>

#define CELL(X, Y) cell_[(Y) * ncellsx_ + (X)]

using namespace Pineapple;
namespace Pocky {

PockyGame::PockyGame(const PockyGameParams &params) {
	// TODO Auto-generated constructor stub
	previousTime_ = 0;
	fps_ = 30;
	params_ = params;
	cell_ = new PockyGridCell[(params.gridx * 2 + 1) * (params.gridy * 2 + 1)];
	ncellsx_ = (params.gridx * 2 + 1);
	ncellsy_ = (params.gridy * 2 + 1);
	for (int i = 0; i < (ncellsx_ * ncellsy_); i++) {
		cell_[i].life = -1;
	}
}

PockyGame::~PockyGame() {
	delete[] cell_;
}

GLFramebufferObject *fbo2;
void PockyGame::init() {
	this->applyGLSettings();
	this->loadShaders();
	this->generateAssets();

	int w = GL::instance()->width();
	int h = GL::instance()->height();
	square_ = new GLQuad(Float3(1, 1, 1), Float3(0.f, 0.f, -5.f),
			Float3(1.f, 1.f, 1.f));
	quad_ = GL::instance()->primitive("quad");
	topbar_ = new GLQuad(Float3(1, 1, 1), Float3(w / 2, 15, 0.f),
			Float3(w, 30, 1.f));
	//botbar_ = new GLQuad(Float3(1, 1, 1), Float3(w/2,h-10, 0.f), Float3(w, 20, 1.f), true);

	glViewport(0, 0, GL::instance()->width(), GL::instance()->height());

//	CELL(5, 3).life = 1.f;
//	CELL(1, 2).life = 0.4f;
//	CELL(3, 0).life = 0.2f;
//	CELL(3, 1).life = 0.7f;
//	CELL(3, 2).life = 1.4f;
//	CELL(4, 0).life = 1.2f;
//	CELL(0, 1).life = 1.7f;
	GL::instance()->perspective(60.f, 0.01f, 1000.f, GL::instance()->width(),
			GL::instance()->height());

	//glClearColor(0.1f, 0.1f, 0.1f, 1.f);
	Float3 p1 = GL::instance()->project(float2(0.f, 0.f), -5.f);
	Float3 p2 = GL::instance()->project(float2(800.f, 480.f), -5.f);

	LOGI("[%f, %f, %f]", p1.x, p1.y, p1.z);
	LOGI("[%f, %f, %f]", p2.x, p2.y, p2.z);

//	Audio::instance()->addSound("test", "assets/audio/short.ogg", true,
//			AudioType::OGG);
//	//Audio::instance()->addSound("test", "assets/audio/technika2.wav", true, AudioType::WAV);
//	Audio::instance()->playSound("test");
}

float prog = 0.f;

void PockyGame::draw(int time) {
	glClear(GL_COLOR_BUFFER_BIT);

	int dt = time - previousTime_;
	if (dt > 0) {
		fps_ = 0.99f * fps_ + 0.01f * (1000 / dt);
		previousTime_ = time;
	}
	int w = GL::instance()->width();
	int h = GL::instance()->height();

	//draw stuff behind the overlay
	GL::instance()->perspective(60.f, 0.01f, 1000.f, GL::instance()->width(),
			GL::instance()->height());
	VSML::instance()->scale(9.6f, 5.97f, 1.f);
	//VSML::instance()->rotate(time / 100.f, 0.f, 1.f, 0.f);
//	bg_->bind(VSML::instance());
//	glActiveTexture(GL_TEXTURE0);
//	framebuffer1_->bindsurface(0);
//	bg_->setUniformValue("tex", 0);
//	float2 scale = {w / 1024.f, h / 1024.f};
//	bg_->setUniformValue("translate", 0.001f);
//	bg_->setUniformValue("texScale", scale);
//	square_->draw(bg_);
//	bg_->release();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR);
	int nLights = 0;
	//float color = sinf(time / 1000.f) * 2.f;
	Engine::instance()->lock();
	for (int i = 0; i < ncellsx_ * ncellsy_; i++) {
		if (cell_[i].life > 0.f && nLights < MAX_ACTIVE) {
			lightPositions_[nLights].x = cell_[i].sspos.x;
			lightPositions_[nLights].y = cell_[i].sspos.y;
			lightPositions_[nLights].z = 20.f * (cell_[i].life + 0.5f);
			nLights++;
			GL::instance()->perspective(60.f, 0.01f, 1000.f,
					GL::instance()->width(), GL::instance()->height());
			VSML::instance()->scale(MAX(cell_[i].life + 0.5f, 1.f)
			, MAX(cell_[i].life+0.5f, 1.f), 1.f);
			VSML::instance()->translate(cell_[i].wspos.x, cell_[i].wspos.y,
					0.f);
			float2 tc(cell_[i].sspos.x / w, 1.f - cell_[i].sspos.y / h);
			hexShader_->bind(VSML::instance());
			glActiveTexture(GL_TEXTURE0);
			framebuffer0_->bindsurface(0);
			hexShader_->setUniformValue("tex", 0);
			float v = (cell_[i].life - 0.5f) * 2.f;
			if (cell_[i].life <= 0.45f)
				hexShader_->setUniformValue("life", -(v * v) + 1.5f);
			else
				hexShader_->setUniformValue(
						"life",
						-((cell_[i].life - 0.5f) * (cell_[i].life - 0.5f))
								+ 0.7f);
			hexShader_->setUniformValue("tcOffset", tc);
			square_->draw(hexShader_);
			hexShader_->release();
		} else if(cell_[i].life > -1.f && cell_[i].life <= 0.f && nLights < MAX_ACTIVE) {
			lightPositions_[nLights].x = cell_[i].sspos.x;
			lightPositions_[nLights].y = cell_[i].sspos.y;
			lightPositions_[nLights].z = 20.f * (cell_[i].life + 1.5f);
			nLights++;
			GL::instance()->perspective(60.f, 0.01f, 1000.f,
					GL::instance()->width(), GL::instance()->height());
			VSML::instance()->translate(cell_[i].wspos.x, cell_[i].wspos.y,
					0.f);
			VSML::instance()->scale(MAX(-cell_[i].life + 1.f, 1.f)
			, MAX(-cell_[i].life+1.f, 1.f), 1.f);

			float2 tc(cell_[i].sspos.x / w, 1.f - cell_[i].sspos.y / h);
			hexShader_->bind(VSML::instance());
			glActiveTexture(GL_TEXTURE0);
			framebuffer0_->bindsurface(0);
			hexShader_->setUniformValue("tex", 0);
			hexShader_->setUniformValue(
						"life",
						-((cell_[i].life - 0.5f) * (cell_[i].life - 0.5f))
								+ 1.5f);
			hexShader_->setUniformValue("tcOffset", tc);
			square_->draw(hexShader_);
			hexShader_->release();
		}
	}

	Engine::instance()->unlock();
	// draw background
	GL::instance()->ortho();
	float2 scale1 = { w / 1024.f, h / 1024.f };
	glActiveTexture(GL_TEXTURE0);
	framebuffer1_->bindsurface(0);
	texLight_->bind(VSML::instance());
	texLight_->setUniformValue("tex", 0);
	texLight_->setUniformValue("nLights", nLights);
	texLight_->setUniformValue("lightpositions", lightPositions_, 10);
	texLight_->setUniformValue("texScale", scale1);
	quad_->draw(texLight_);
	texLight_->release();

	//overlay
	prog += dt / 100000.f;
	glBlendFunc(GL_SRC_COLOR, GL_ONE_MINUS_SRC_ALPHA);
	overlay_->bind(VSML::instance());
	overlay_->setUniformValue("height", 1.f / 30.f);
	overlay_->setUniformValue("progress", (float) Audio::instance()->getPercentComplete("sim"));
	topbar_->draw(overlay_);
	overlay_->release();
	glDisable(GL_BLEND);
	std::stringstream ss;
	ss << std::setfill('0') << std::setw(9) << score_;//"FPS > " << (int) fps_; // << " <> " << progress;// << "\nRES > " << GL::instance()->width() << " X " << GL::instance()->height();
	GL::instance()->renderText(ss.str(), Float3(2.f, -7.f, 0.f),
			FONTS::FontLekton);
//	glEnable(GL_BLEND);
//	glBlendFunc(GL_SRC_COLOR, GL_ONE_MINUS_SRC_ALPHA);
//
//	overlay_->bind(VSML::instance());
//	overlay_->setUniformValue("height", 0.f);
//	overlay_->setUniformValue("progress", 50.f / (float)w);
//	quad_->draw(overlay_);
//	overlay_->release();
//	GL::instance()->renderText("SONG SELECT", Float3(70.f, 5.f, 0.f), FONTS::FontLekton);
//
//	GL::instance()->renderText("I WANT TO BE THE VERY BEST\n> 3:00 / ASH KETCHUM", Float3(70.f, 55.f, 0.f), FONTS::FontLekton);
//	GL::instance()->renderText("I WANT TO BE THE VERY BEST\n> 3:00 / ASH KETCHUM", Float3(70.f, 155.f, 0.f), FONTS::FontLekton);
	//GL::instance()->renderText("P\nI\nD\nG\nE\nY", Float3(15.f, 5.f, 0.f), FONTS::FontLekton);
}

void IdxToRGB565(int idx, Float3 &rgb) {
	int r = idx / (32 * 64);
	int g = (idx - r * 32 * 64) / 32;
	int b = (idx - r * 32 * 64 - g * 32);
	rgb.x = r * 8;
	rgb.y = g * 4;
	rgb.z = b * 8;
	rgb /= 255.f;
}

void PockyGame::DrawGrid(int radx, int rady, bool solid) {

	GLPrimitive *square = new GLQuad(Float3(10, 10, 10), Float3(0.f, 0.f, -5.f),
			Float3(1.f, 1.f, 1.f));
	if (!solid) {
		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ONE);
	} else {
		glDisable(GL_BLEND);
	}
	GLPrimitive *disc = new GLDisc(Float3(6, 1, 1), Float3(0.f, 0.f, -5.f),
			Float3(1.f, 1.f, 1.f));
	float2 scale = { 1.f, 1.f };
	for (int y = -rady, i = 0, j = 0; y <= rady; y++, i++) {
		for (int x = -radx; x <= radx; x++, j++) {
//			cell_[j].life = 0.f;
			cell_[j].id = j + 1;
			GL::instance()->perspective(60.f, 0.01f, 1000.f,
					GL::instance()->width(), GL::instance()->height());
			if (i % 2 == 0) {
				cell_[j].wspos = Float3(x * (1.05), -y * 0.95, -5.f);
				VSML::instance()->translate(x * (1.05), y * 0.95, 0.f);
			} else {
				cell_[j].wspos = Float3(x * (1.05) + 0.5f, -y * 0.95, -5.f);
				VSML::instance()->translate(x * (1.05) + 0.5f, y * 0.95, 0.f);
			}
			cell_[j].sspos = GL::instance()->unproject(cell_[j].wspos);
			if (solid) {
				GL::instance()->shader("id")->bind(VSML::instance());
				//convert j=1...65535 to r5g6b5
				//32 - 64 - 32
				//r*32*64+g*32+b
				//r = idx / (32*64), g = (idx - r*32*64) / 32, b=(idx - r*32*64-g*32)
				Float3 color;
				IdxToRGB565(cell_[j].id, color);
				GL::instance()->shader("id")->setUniformValue("id", color);
				disc->draw("id");
				GL::instance()->shader("id")->release();
			} else {
				GL::instance()->shader("texmap")->bind(VSML::instance());
				glActiveTexture(GL_TEXTURE0);
				fbo2->bindsurface(0);
				GL::instance()->shader("texmap")->setUniformValue("tex", 0);
				GL::instance()->shader("texmap")->setUniformValue("texScale",
						scale);
				square->draw("texmap");
				GL::instance()->shader("texmap")->release();
			}
		}
	}
	delete disc;
	delete square;
	glDisable(GL_BLEND);
}

void PockyGame::generateAssets() {
	GL::instance()->createShader("blur", "assets/shaders/blur.glsl");
	GLFramebufferObjectParams parms;
	parms.width = 128;
	parms.height = 128;
	parms.type = GL_TEXTURE_2D;
	parms.format = GL_RGBA;
	parms.hasDepth = false;
	framebuffer0_ = new GLFramebufferObject(parms);
	fbo2 = new GLFramebufferObject(parms);

	framebuffer0_->bind();
	glClearColor(0.0f, 0.f, 0.f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glViewport(0, 0, parms.width, parms.height);
	GL::instance()->ortho(parms.width, parms.height);
	GL::instance()->shader("default")->bind(VSML::instance());
	for (int i = 0; i < 2; i++) {
		GLPrimitive *hexagon = new GLCircle(
				Float3(6, 10, 10),
				Float3(parms.width / 2.f, parms.height / 2.f, 0.f),
				Float3(parms.width - i * 20 - 2, parms.height - i * 20 - 2,
						1.f));
		hexagon->draw("default");
		delete hexagon;
	}
	GL::instance()->shader("default")->release();
	framebuffer0_->release();

	GLQuad *fx = new GLQuad(Float3(10, 10, 10),
			Float3(parms.width / 2, parms.height / 2, 1.f),
			Float3(parms.width, parms.height, 1.f));

	fbo2->bind();
	//antialiasing pass
	GL::instance()->shader("blur")->bind(VSML::instance());
	glActiveTexture(GL_TEXTURE0);
	framebuffer0_->bindsurface(0);
	GL::instance()->shader("blur")->setUniformValue("tex", 0);
	fx->draw("blur");
	fbo2->release();
	delete framebuffer0_;
	parms.width = 1024;
	parms.height = 1024;
	glClearColor(0.f, 0.f, 0.0f, 1.0f);
	glViewport(0, 0, GL::instance()->width(), GL::instance()->height());
	framebuffer1_ = new GLFramebufferObject(parms);
	//framebuffer1_->bind();

	glClear(GL_COLOR_BUFFER_BIT);
	glDisable(GL_DITHER);
	GL::instance()->perspective(60.f, 0.01f, 1000.f, GL::instance()->width(),
			GL::instance()->height());
	DrawGrid(params_.gridx, params_.gridy, true);

#ifndef _DESKTOP
	GLushort *texdata = new GLushort[GL::instance()->width()
			* GL::instance()->height()];
	ids_ = new int[GL::instance()->width() * GL::instance()->height()];
	glReadPixels(0, 0, GL::instance()->width(), GL::instance()->height(),
			GL_RGB, GL_UNSIGNED_SHORT_5_6_5, texdata);
#else

	GLbyte *texdata = new GLbyte[GL::instance()->width() * GL::instance()->height()*4];
	ids_ = new int[GL::instance()->width() * GL::instance()->height()];
	glReadPixels(0, 0, GL::instance()->width(), GL::instance()->height(), GL_RGBA, GL_BYTE, texdata);
#endif
	for (int y = 0, i = 0; y < GL::instance()->height(); y++) {
		for (int x = 0; x < GL::instance()->width(); x++, i++) {
#ifndef _DESKTOP
			int r = (255 * ((texdata[i]) >> 11) + 15) / 31;
			int g = (255 * ((texdata[i] & 0x7E0) >> 5) + 31) / 63;
			int b = (255 * ((texdata[i] & 0x01F)) + 15) / 31;
			ids_[i] = (r * 32 * 64 / 8 + g * 32 / 4 + b / 8) - 1;
#else
			int r = texdata[i*4];
			int g = texdata[i*4+1];
			int b = texdata[i*4+2];
			ids_[i] = (r*32*64/8+g*32/4+b/8)-1;
//			if(r!=0||g!=0||b!=0)
//				qDebug() << "test:: " << r << ", " << g << ", " << b << ":::" << ids_[i];
#endif
		}
	}
	delete[] texdata;

	framebuffer1_->bind();
	glClear(GL_COLOR_BUFFER_BIT);
	GL::instance()->perspective(60.f, 0.01f, 1000.f, GL::instance()->width(),
			GL::instance()->height());
	DrawGrid(params_.gridx, params_.gridy, false);

	framebuffer1_->release();

	//begine active hex generation
	framebuffer1_->releaseFramebuffer();
	parms.width = 128;
	parms.height = 128;
	framebuffer0_ = new GLFramebufferObject(parms);
	framebuffer0_->bind();
	glClear(GL_COLOR_BUFFER_BIT);
	glViewport(0, 0, parms.width, parms.height);
	GL::instance()->ortho(parms.width, parms.height);
	GLPrimitive *hexagon = new GLDisc(Float3(6, 10, 10),
			Float3(parms.width / 2.f, parms.height / 2.f, 0.f),
			Float3(parms.width - 20, parms.height - 20, 1.f));

	GL::instance()->shader("default2")->bind(VSML::instance());
	glActiveTexture(GL_TEXTURE0);
	fbo2->bindsurface(0);
	GL::instance()->shader("default2")->setUniformValue("tex", 0);
	hexagon->draw("default2");
	GL::instance()->shader("default2")->release();

	delete hexagon;
	framebuffer0_->release();
	delete fx;
	delete fbo2;
}

void PockyGame::loadShaders() {
	GL::instance()->createShader("default2", "assets/shaders/default2.glsl"); //i have no fucking clue why this is necessary to prevent a segfault on the desktop...just deal with it - paul
	GL::instance()->createShader("hex", "assets/shaders/hex.glsl");
	hexShader_ = GL::instance()->shader("hex");
	GL::instance()->createShader("texlit", "assets/shaders/texmaplit.glsl");
	texLight_ = GL::instance()->shader("texlit");
	GL::instance()->createShader("bg", "assets/shaders/background.glsl");
	bg_ = GL::instance()->shader("bg");
//	GL::instance()->createShader("alpha", "assets/shaders/alpha.glsl");
//	bg_ = GL::instance()->shader("alpha");
	GL::instance()->createShader("overlay", "assets/shaders/overlay.glsl");
	overlay_ = GL::instance()->shader("overlay");

	GL::instance()->createShader("id", "assets/shaders/id.glsl");
	id_ = GL::instance()->shader("id");
}

void PockyGame::applyGLSettings() {
	glEnable(GL_TEXTURE_2D);
	glClearColor(0.f, 0.f, 0.f, 1.0f);
	glClearColor(0.f, 0.f, 0.f, 1.0f);
	glDisable(GL_DEPTH_TEST);
	glLineWidth(1.5f);
}
int PockyGame::getGridLocation(int x, int y) {
	return ids_[y * GL::instance()->width() + x];
}

} /* namespace Pineapple */
