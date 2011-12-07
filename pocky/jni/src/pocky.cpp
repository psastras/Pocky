/*
 * pocky.cpp
 *
 *  Created on: Nov 28, 2011
 *      Author: psastras
 */
#include "../include/pocky_libs_Pocky.h"
#include "../include/PockyGame.h"
#include "../include/PockyState.h"
#include <pineapple/jni/extern/Engine.h>
#include <pineapple/jni/extern/Compile.h>
#include <pineapple/jni/extern/Common.h>
#include <pineapple/jni/extern/GL.h>
#include <pineapple/jni/extern/GLText.h>
#include <pineapple/jni/extern/Audio.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <sstream>

using namespace Pineapple;
using namespace Pocky;

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void * reserved)
{
    return JNI_VERSION_1_6;
}

PockyGame *g_Game;
PockyState *g_State;

JNIEXPORT void JNICALL Java_pocky_libs_Pocky_startup
  (JNIEnv *, jclass) {

	LOGI("Pocky Started");
	//size_t size;
	//unsigned char * data = Engine::instance()->readResourceFromAPK("assets/audio/woman.wav", size);
//	LOGI("Creating Audio Object");
	//Audio *aud = new Audio(data, size);
	GL::instance()->init();

	//int w;
	PockyGameParams params;
	params.gridx = 5;
	params.gridy = 3;
	g_Game  = new PockyGame(params);
	g_State = new PockyState(g_Game);
	Engine::instance()->setUpdatable(g_State);
	//Engine::instance()->readPNGFromAPK("assets/textures/cat.png", &w, &h);
	//LOGI("%d, %d", w, h);
}


JNIEXPORT void JNICALL Java_pocky_libs_Pocky_resize
  (JNIEnv *, jclass, jint w, jint h) {
	// load universal shaders

	GL::instance()->createShader("texmap", "assets/shaders/texmap.glsl");
	GL::instance()->createShader("default", "assets/shaders/default.glsl");

	GL::instance()->initializeGL(w, h);
	g_Game->init();
//	GL::instance()->loadFont(FONTS::FontRobotoRegular);
}


JNIEXPORT void JNICALL Java_pocky_libs_Pocky_draw
  (JNIEnv *, jclass, jint time) {
	g_Game->draw(time);

	glFlush();
}

JNIEXPORT void JNICALL Java_pocky_libs_Pocky_onTouch
  (JNIEnv *, jclass, jfloat x , jfloat y) {
	LOGI("ID [%d, %d]: %d", (int)x, (int)y, g_Game->getGridLocation(x, y));
}


JNIEXPORT void JNICALL Java_pocky_libs_Pocky_onDrag
  (JNIEnv *, jclass, jfloat x , jfloat y) {
	LOGI("ID [%d, %d]: %d", (int)x, (int)y, g_Game->getGridLocation(x, y));
}


JNIEXPORT void JNICALL Java_pocky_libs_Pocky_onRelease
  (JNIEnv *, jclass, jfloat, jfloat) {
	//LOGI("Release");
}


JNIEXPORT void JNICALL Java_pocky_libs_Pocky_shutdown
  (JNIEnv *, jclass) {
	LOGI("Pocky Shutdown");
}
