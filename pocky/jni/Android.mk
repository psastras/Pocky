#WARNING THIS Android.mk IS AUTOGENERATED (DO NOT MODIFY)

LOCAL_PATH:= $(call my-dir)
 include $(CLEAR_VARS)
LOCAL_MODULE    := pocky
LOCAL_CFLAGS    := -Werror -I../
LOCAL_SRC_FILES :=  src/pocky.cpp
LOCAL_LDLIBS    := -llog -lGLESv2  -L../pineapple -lpineapple
include $(BUILD_SHARED_LIBRARY)
