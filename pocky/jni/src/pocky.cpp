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
	LOGI("Creating Audio Object");
	Audio *aud = new Audio(data, size);
	GL::instance()->init();
}

GLuint tex;

JNIEXPORT void JNICALL Java_pocky_libs_Pocky_resize
  (JNIEnv *, jclass, jint w, jint h) {
	glEnable(GL_TEXTURE_2D);
	GL::instance()->createShader("default", "assets/shaders/default.glsl");
	GL::instance()->createShader("texmap", "assets/shaders/texmap.glsl");
	GL::instance()->initializeGL(w, h);
	GLFramebufferObjectParams parms;
	parms.hasDepth = false;
	int maxdim = NextPowerOfTwo(MAX(w, h));

	parms.width = maxdim;
	parms.height = maxdim;
	LOGI("DIM:%d", maxdim);
	parms.type = GL_TEXTURE_2D;
	parms.format = GL_RGBA;
	fbo = new GLFramebufferObject(parms);
	plane = new GLPlane(Float3(10, 10, 10), Float3::zero(), Float3(5.f, 5.f, 5.f));
	quad = new GLQuad(Float3(10, 10, 10), Float3(w/2,h/2,1.f), Float3(w, h, 1.f));
	glClearColor(1.f, 0.1f, 0.1f, 1.0f);
	int texsize = NextPowerOfTwo(100);
	unsigned char *data = new unsigned char[texsize*texsize*4];
	for(int i=0; i<texsize*texsize*4;i++) {
		data[i]= 255;
	}
	LOGI("%d", GL::instance()->shader("texmap")->getUniformLocation("tex"));
	 if(glGetError() != GL_NO_ERROR) {
		 LOGE("GLERR1");
	 }
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texsize, texsize, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glBindTexture(GL_TEXTURE_2D, 0);
	delete[] data;



	 if(glGetError() != GL_NO_ERROR) {
		 LOGE("GLERR2");
	 }
}


JNIEXPORT void JNICALL Java_pocky_libs_Pocky_draw
  (JNIEnv *, jclass, jint time) {
	return;
	 fbo->bind();
	 glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
//	 GL::instance()->perspective(60.f, 0.01f, 1000.f);
//	 VSML::instance()->translate(0.f, 2.f, -10.f);
//	 VSML::instance()->rotate(time / 100.f, 1.f, 1.f, 0.f);
//	 plane->draw("default");
	 fbo->release();

	 glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	 if(glGetError() != GL_NO_ERROR) {
		 LOGE("GLERRBEGIN");
		 exit(0);
	 }
	 GL::instance()->ortho();
	 GL::instance()->shader("texmap")->bind(VSML::instance());

	 glActiveTexture(GL_TEXTURE0);
	 //fbo->bindsurface(0);
	 glBindTexture(GL_TEXTURE_2D, tex);
	 GL::instance()->shader("texmap")->setUniformValue("tex", 0);
	 quad->draw("texmap");
	 glBindTexture(GL_TEXTURE_2D, 0);
	 GL::instance()->shader("texmap")->release();

	 if(glGetError() != GL_NO_ERROR) {
		 LOGE("GLERREND");
		 exit(0);
	 }
}

JNIEXPORT void JNICALL Java_pocky_libs_Pocky_shutdown
  (JNIEnv *, jclass) {
	LOGI("Pocky Shutdown");
}
