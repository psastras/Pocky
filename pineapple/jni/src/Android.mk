LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../libzip/
LOCAL_STATIC_LIBRARIES := libzip 
LOCAL_MODULE    := pineapple
LOCAL_CFLAGS    := -Werror 
LOCAL_SRC_FILES := VSML.cpp GLPrimitive.cpp GLShaderProgram.cpp GL.cpp \
				   GLShader.cpp PineappleLib.cpp  Engine.cpp SceneObject.cpp Scene.cpp #$(wildcard *.cpp)
LOCAL_LDLIBS    := -llog -lGLESv2 -ldl -lz
include $(BUILD_SHARED_LIBRARY)