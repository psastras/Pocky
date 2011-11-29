/*
 * pocky.cpp
 *
 *  Created on: Nov 28, 2011
 *      Author: psastras
 */
#include "../include/pocky_libs_Pocky.h"

#include <pineapple/jni/extern/Common.h>
#include <pineapple/jni/extern/GL.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
using namespace Pineapple;

GL *pGL = 0;

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
	pGL->initializeGL(w, h);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
}


JNIEXPORT void JNICALL Java_pocky_libs_Pocky_draw
  (JNIEnv *, jclass) {
	 glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
}

JNIEXPORT void JNICALL Java_pocky_libs_Pocky_shutdown
  (JNIEnv *, jclass) {
	LOGI("Pocky Shutdown");
}
