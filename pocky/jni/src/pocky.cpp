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
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
using namespace Pineapple;

GL *pGL = 0;
GLShaderProgram *prog;
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
	RELEASE(pGL);
	pGL = new GL();
}

JNIEXPORT void JNICALL Java_pocky_libs_Pocky_resize
  (JNIEnv *, jclass, jint w, jint h) {
	char *data = Pineapple::Engine::instance()->readResourceFromAPK("assets/shaders/default.glsl");
	LOGI("%s", data);
	pGL->initializeGL(w, h);
	GLFramebufferObjectParams parms;
	parms.hasDepth = false;
	parms.width = w;
	parms.height = h;
	parms.type = GL_TEXTURE_2D;
	parms.format =GL_RGBA;
	fbo = new GLFramebufferObject(parms);
	prog = new GLShaderProgram();
	prog->loadShaderFromData(GL_FRAGMENT_SHADER, data);
	prog->loadShaderFromData(GL_VERTEX_SHADER, data);
	prog->link();
	delete[] data;
	plane = new GLPlane(Float3(10, 10, 10), Float3::zero(), Float3(5.f, 5.f, 5.f));
	quad = new GLQuad(Float3(10, 10, 10), Float3::zero(), Float3(5.f, 5.f, 5.f));
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
}


JNIEXPORT void JNICALL Java_pocky_libs_Pocky_draw
  (JNIEnv *, jclass, jint time) {
	 glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	 pGL->perspective(60.f, 0.01f, 1000.f);
	 VSML::instance()->translate(0.f, 2.f, -10.f);
	 //VSML::instance()->rotate(time / 100.f, 1.f, 1.f, 0.f);
	 quad->draw(prog);

}

JNIEXPORT void JNICALL Java_pocky_libs_Pocky_shutdown
  (JNIEnv *, jclass) {
	LOGI("Pocky Shutdown");
	RELEASE(pGL)
}
