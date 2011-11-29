#include "../include/pineapple_libs_PineappleLib.h"
#include "../extern/Common.h"
#include "../extern/Engine.h"
#include "../extern/GL.h"

Pineapple::Engine *Pineapple::Engine::s_instance = 0;
JavaVM* pJVM = 0;

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void * reserved)
{
	pJVM = vm;
    return JNI_VERSION_1_6;
}

void JNI_OnUnload(JavaVM *vm, void *reserved)
{
	pJVM = 0;
}


JNIEXPORT void JNICALL Java_pineapple_libs_PineappleLib_init
  (JNIEnv *, jclass) {

	Pineapple::Engine::init();

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
