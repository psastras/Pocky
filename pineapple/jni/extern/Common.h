#ifndef COMMON_H_
#define COMMON_H_

#include <jni.h>
#include <android/log.h>
#define  LOG_TAG    "libpineapplejni"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
#define  RELEASE(X) \
	if(X) delete X; \
	X = 0;
#define MAX(x, y) (x) > (y) ? (x) : (y)
#define MIN(x, y) (x) < (y) ? (x) : (y)

static int NextPowerOfTwo(int val) {
	val--;
	val = (val >> 1) | val;
	val = (val >> 2) | val;
	val = (val >> 4) | val;
	val = (val >> 8) | val;
	val = (val >> 16) | val;
	val++; // Val is now the next highest power of 2.
	return val;
}

#endif /* COMMON_H_ */
