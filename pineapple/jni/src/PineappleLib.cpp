#include "../include/pineapple_libs_PineappleLib.h"
#include "../extern/Common.h"
#include "../extern/Engine.h"
#include "../extern/GL.h"
#include "../libzip/zip.h"
Pineapple::Engine *Pineapple::Engine::s_instance = 0;
Pineapple::GL *Pineapple::GL::s_instance = 0;
JavaVM* pJVM = 0;

static void loadAPK (const char* apkPath) {
  LOGI("Loading APK %s", apkPath);
  Pineapple::Engine::instance()->setAPKArchive(zip_open(apkPath, 0, NULL));
}

//JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void * reserved)
//{
//	pJVM = vm;
//    return JNI_VERSION_1_6;
//}

void JNI_OnUnload(JavaVM *vm, void *reserved)
{
	pJVM = 0;
}


JNIEXPORT void JNICALL Java_pineapple_libs_PineappleLib_init
  (JNIEnv *env, jclass, jstring apkPath) {
	Pineapple::Engine::init();
	const char* str;
	jboolean isCopy;
	str = env->GetStringUTFChars(apkPath, &isCopy);
	loadAPK(str);
	LOGI("Pineapple Library Initialized");

}



JNIEXPORT void JNICALL Java_pineapple_libs_PineappleLib_test
  (JNIEnv *, jclass) {

}


JNIEXPORT void JNICALL Java_pineapple_libs_PineappleLib_start
  (JNIEnv *, jclass) {
	Pineapple::Engine::instance()->start();
}


JNIEXPORT void JNICALL Java_pineapple_libs_PineappleLib_stop
  (JNIEnv *, jclass) {
	Pineapple::Engine::instance()->stop();
}
