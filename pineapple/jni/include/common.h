/*
 * common.h
 *
 *  Created on: Nov 28, 2011
 *      Author: psastras
 */

#ifndef COMMON_H_
#define COMMON_H_

#include <jni.h>
#include <android/log.h>
#define  LOG_TAG    "libpineapplejni"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

#endif /* COMMON_H_ */
