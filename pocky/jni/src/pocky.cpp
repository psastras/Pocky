/*
 * pocky.cpp
 *
 *  Created on: Nov 28, 2011
 *      Author: psastras
 */
#include "../include/pocky_libs_Pocky.h"
#include <pineapple/jni/extern/Engine.h>
#include <pineapple/jni/extern/Common.h>
#include <pineapple/jni/extern/GL.h>
#include <pineapple/jni/extern/GLText.h>
#include <pineapple/jni/extern/Audio.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

using namespace Pineapple;

GLPlane *plane;
GLQuad *quad;
GLFramebufferObject *fbo;
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void * reserved)
{
    return JNI_VERSION_1_6;
}

JNIEXPORT void JNICALL Java_pocky_libs_Pocky_startup
  (JNIEnv *, jclass) {

	LOGI("Pocky Started");
	size_t size;
	unsigned char * data = Engine::instance()->readResourceFromAPK("assets/audio/woman.wav", size);
//	LOGI("Creating Audio Object");
	//Audio *aud = new Audio(data, size);
	GL::instance()->init();
}

JNIEXPORT void JNICALL Java_pocky_libs_Pocky_resize
  (JNIEnv *, jclass, jint w, jint h) {
	glEnable(GL_TEXTURE_2D);

	GL::instance()->createShader("default", "assets/shaders/default.glsl");
	GL::instance()->createShader("texmap", "assets/shaders/texmap.glsl");
	GL::instance()->initializeGL(w, h);
	GL::instance()->loadFont(FONTS::FontRobotoRegular);
	GLFramebufferObjectParams parms;

	int maxdim = NextPowerOfTwo(MAX(w, h));

	parms.width = maxdim;
	parms.height = maxdim;
	parms.type = GL_TEXTURE_2D;
	parms.format = GL_RGBA;
	parms.hasDepth = true;
	fbo = new GLFramebufferObject(parms);
	plane = new GLPlane(Float3(10, 10, 10), Float3::zero(), Float3(5.f, 5.f, 5.f));
	quad = new GLQuad(Float3(10, 10, 10), Float3(w/2,h/2,1.f), Float3(w, h, 1.f));
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
}


JNIEXPORT void JNICALL Java_pocky_libs_Pocky_draw
  (JNIEnv *, jclass, jint time) {

	// Render to the framebuffer
	 fbo->bind();
	 glEnable(GL_DEPTH_TEST);
	 glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	 GL::instance()->perspective(60.f, 0.01f, 1000.f);
	 VSML::instance()->translate(0.f, 2.f, -10.f);
	 VSML::instance()->rotate(time / 100.f, 1.f, 1.f, 0.f);
	 GL::instance()->shader("texmap")->bind(VSML::instance());
	 glActiveTexture(GL_TEXTURE0);
	 GL::instance()->fontTexture(FONTS::FontRobotoRegular)->id();
	 GL::instance()->shader("texmap")->setUniformValue("tex", 0);
	 plane->draw("texmap");
	 GL::instance()->shader("texmap")->release();
	 fbo->release();

	 // Draw the framebuffer to screen.
	 glDisable(GL_DEPTH_TEST);
	 GL::instance()->ortho();
	 GL::instance()->shader("texmap")->bind(VSML::instance());
	 glActiveTexture(GL_TEXTURE0);
	 fbo->bindsurface(0);
	 float2 scale = {GL::instance()->width() / (float)fbo->width(),
			 GL::instance()->height() / (float)fbo->height()};
	 GL::instance()->shader("texmap")->setUniformValue("texScale", scale);
	 GL::instance()->shader("texmap")->setUniformValue("tex", 0);
	 quad->draw("texmap");
	 glBindTexture(GL_TEXTURE_2D, 0);
	 GL::instance()->shader("texmap")->release();
}

JNIEXPORT void JNICALL Java_pocky_libs_Pocky_shutdown
  (JNIEnv *, jclass) {
	LOGI("Pocky Shutdown");
}
