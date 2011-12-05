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
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <sstream>

#define CELL(X, Y) cell_[(Y) * ncellsx_ + (X)]

using namespace Pineapple;
namespace Pocky {

PockyGame::PockyGame(const PockyGameParams &params) {
	// TODO Auto-generated constructor stub
	previousTime_ = 0;
	fps_ = 30;
	params_ = params;
	cell_ = new PockyGridCell[(params.gridx*2+1) * (params.gridy*2+1)];
	ncellsx_ = (params.gridx*2+1);
	ncellsy_ = (params.gridy*2+1);
}

PockyGame::~PockyGame() {
	delete[] cell_;
}

float touchx = 400.f;
float touchy = 240.f;

GLFramebufferObject *fbo2;
void PockyGame::init() {
	this->applyGLSettings();
	this->loadShaders();
	this->generateAssets();

	int w = GL::instance()->width();
	int h = GL::instance()->height();
	square_ = new GLQuad(Float3(10, 10, 10), Float3(0.f,0.f, -5.f), Float3(1.f, 1.f, 1.f));
	quad_ = GL::instance()->primitive("quad");
	glViewport(0, 0, GL::instance()->width(), GL::instance()->height());

	CELL(5, 3).life = 1.f;
	CELL(1, 2).life = 1.f;
	CELL(3, 0).life = 1.f;
	GL::instance()->perspective(60.f, 0.01f, 1000.f, GL::instance()->width(), GL::instance()->height());
		VSML::instance()->translate(2*(1.05)+0.5f, 0*0.95, 0.f);
		Float3 wspos(2*(1.05)+0.5f, 0*0.95, -5.f);
		const float2& sspos = GL::instance()->unproject(wspos);
	LOGI("[%f, %f]", sspos.x, sspos.y);
}

void PockyGame::draw(int time) {
	glClear(GL_COLOR_BUFFER_BIT);
	int dt = time - previousTime_;
    fps_ = 0.99f * fps_ + 0.01f * (1000 / dt);
	previousTime_ = time;
	int w = GL::instance()->width();
	int h = GL::instance()->height();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_COLOR, GL_DST_ALPHA);
	int nLights = 0;

	for(int i=0; i<ncellsx_*ncellsy_; i++)
	{
		if(cell_[i].life > 0.f && nLights < MAX_ACTIVE)
		{
			lightPositions_[nLights].x = cell_[i].sspos.x;
			lightPositions_[nLights].y = cell_[i].sspos.y;
			lightPositions_[nLights].z = 20.f;
			nLights++;
			GL::instance()->perspective(60.f, 0.01f, 1000.f, GL::instance()->width(), GL::instance()->height());
			VSML::instance()->translate(cell_[i].wspos.x, cell_[i].wspos.y, 0.f);
			float2 tc(cell_[i].sspos.x / w, 1.f - cell_[i].sspos.y / h);
			hexShader_->bind(VSML::instance());
			glActiveTexture(GL_TEXTURE0);
			framebuffer0_->bindsurface(0);
			hexShader_->setUniformValue("tex", 0);
			hexShader_->setUniformValue("tcOffset", tc);
			square_->draw(hexShader_);
			hexShader_->release();

			cell_[i].life -= dt / 1000.f;
			if(cell_[i].life <= 0.f) {
						while(true) {
							int idx = rand() % (ncellsx_ * ncellsy_);
							if(cell_[idx].life <= 0) {
								cell_[idx].life = 1.f;
								break;
						}
					}
		}
	}

	//update step


	}
	GL::instance()->ortho();
	float2 scale1 = {w / 1024.f, h / 1024.f};
	glActiveTexture(GL_TEXTURE0);
	framebuffer1_->bindsurface(0);
	texLight_->bind(VSML::instance());
	texLight_->setUniformValue("tex", 0);
	texLight_->setUniformValue("nLights", nLights);
	texLight_->setUniformValue("lightpositions", lightPositions_, 10);
	texLight_->setUniformValue("texScale", scale1);
	quad_->draw(texLight_);
	texLight_->release();
	glDisable(GL_BLEND);
	std::stringstream ss;
	ss << "FPS > " << (int)fps_ << "\nRES > " << GL::instance()->width() << " X " << GL::instance()->height();
	GL::instance()->renderText(ss.str(), FONTS::FontLekton);
}


void PockyGame::DrawGrid(int radx, int rady)
{

	GLPrimitive *square = new GLQuad(Float3(10, 10, 10), Float3(0.f, 0.f, -5.f), Float3(1.f, 1.f, 1.f));
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);
	float2 scale = {1.f, 1.f};
	for(int y=-rady,i=0,j=0; y<=rady; y++, i++)
	{
		for(int x=-radx; x<=radx; x++,j++)
		{
			cell_[j].life = 0.f;


			GL::instance()->perspective(60.f, 0.01f, 1000.f, GL::instance()->width(), GL::instance()->height());
			if(i%2 ==0 ) {
				cell_[j].wspos = Float3(x*(1.05), y*0.95, -5.f);
				VSML::instance()->translate(x*(1.05), y*0.95, 0.f);
			}
			else {
				cell_[j].wspos = Float3(x*(1.05)+0.5f, y*0.95, -5.f);
				VSML::instance()->translate(x*(1.05)+0.5f, y*0.95, 0.f);
			}
			cell_[j].sspos = GL::instance()->unproject(cell_[j].wspos);
			if(x==2 && y == 0)
				LOGI("%f, %f, %d", cell_[j].sspos.x, cell_[j].sspos.y, j);
			GL::instance()->shader("texmap")->bind(VSML::instance());
			glActiveTexture(GL_TEXTURE0);
			fbo2->bindsurface(0);
			GL::instance()->shader("texmap")->setUniformValue("tex", 0);
			GL::instance()->shader("texmap")->setUniformValue("texScale", scale);
			square->draw("texmap");
			GL::instance()->shader("texmap")->release();
			//VSML::instance()->popMatrix(VSML::MODELVIEW);

		}
	}

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
	GLPrimitive *filled = new GLDisc(Float3(6, 10, 10), Float3(parms.width / 2.f, parms.height / 2.f, 0.f),
					Float3(parms.width - 2, parms.height - 2, 1.f));

	for(int i=0; i<2; i++) {
		GLPrimitive *hexagon = new GLCircle(Float3(6, 10, 10), Float3(parms.width / 2.f, parms.height / 2.f, 0.f),
					Float3(parms.width - i*20 - 2, parms.height - i*20 - 2, 1.f));
		hexagon->draw("default");
		delete hexagon;
	}
	GL::instance()->shader("default")->release();
	framebuffer0_->release();

	GLQuad *fx = new GLQuad(Float3(10, 10, 10), Float3(parms.width/2, parms.height/2,1.f), Float3(parms.width,  parms.height, 1.f));

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
	framebuffer1_ = new GLFramebufferObject(parms);
	framebuffer1_->bind();
	glClearColor(0.f, 0.f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glViewport(0, 0, GL::instance()->width(), GL::instance()->height());
	GL::instance()->perspective(60.f, 0.01f, 1000.f, GL::instance()->width(), GL::instance()->height());
	DrawGrid(params_.gridx, params_.gridy);
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
	GL::instance()->shader("default")->bind(VSML::instance());
	GLPrimitive *hexagon = new GLDisc(Float3(6, 10, 10), Float3(parms.width / 2.f, parms.height / 2.f, 0.f),
				Float3(parms.width-20, parms.height-20, 1.f));
	hexagon->draw("default");
	delete hexagon;
	GL::instance()->shader("default")->release();
	framebuffer0_->release();
	delete fx;
	delete fbo2;
}

void PockyGame::loadShaders() {
	GL::instance()->createShader("hex", "assets/shaders/hex.glsl");
	hexShader_ = GL::instance()->shader("hex");
	GL::instance()->createShader("texlit", "assets/shaders/texmaplit.glsl");
	texLight_ = GL::instance()->shader("texlit");
}

void PockyGame::applyGLSettings() {
	glEnable(GL_TEXTURE_2D);
	glClearColor(0.f, 0.f, 0.f, 1.0f);
	glClearColor(0.f, 0.f, 0.f, 1.0f);
	glDisable(GL_DEPTH_TEST);
	glLineWidth(1.f);
}

} /* namespace Pineapple */
