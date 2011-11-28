#include "../include/pineapple_libs_PineappleLib.h"
#include "../include/common.h"

JNIEXPORT void JNICALL Java_pineapple_libs_PineappleLib_init
  (JNIEnv *, jclass) {
	LOGI("TESTING!");
}

JNIEXPORT void JNICALL Java_pineapple_libs_PineappleLib_test
  (JNIEnv *, jclass) {
	LOGI("HELLO WORLD");
}
