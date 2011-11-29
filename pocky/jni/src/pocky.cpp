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
	prog = new GLShaderProgram();
	prog->loadShaderFromData(GL_FRAGMENT_SHADER, data);
	prog->loadShaderFromData(GL_VERTEX_SHADER, data);
	prog->link();
	delete[] data;
	plane = new GLPlane(Float3(10, 10, 10), Float3::zero(), Float3(5.f, 5.f, 5.f));
	VSML::instance()->loadIdentity(VSML::PROJECTION);
	VSML::instance()->perspective(60.f, w / (float)h, 0.01f, 1000.f);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
}


JNIEXPORT void JNICALL Java_pocky_libs_Pocky_draw
  (JNIEnv *, jclass, jint time) {
	 glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	 VSML::instance()->loadIdentity(VSML::MODELVIEW);
	 VSML::instance()->translate(0.f, 2.f, -10.f);
	 VSML::instance()->rotate(time / 100.f, 1.f, 1.f, 0.f);
	 prog->bind(VSML::instance());
	 plane->draw(prog);
	 prog->release();
}

JNIEXPORT void JNICALL Java_pocky_libs_Pocky_shutdown
  (JNIEnv *, jclass) {
	LOGI("Pocky Shutdown");
	RELEASE(pGL)
}
