#-------------------------------------------------
#
# Project created by QtCreator 2011-12-08T13:58:20
#
#-------------------------------------------------

QT       += core gui opengl
DEFINES += _DESKTOP
TARGET = Beatmap
TEMPLATE = app
INCLUDEPATH += ../../
SOURCES += main.cpp \
    glwidget.cpp \
    mainwindow.cpp \
    ../../pineapple/jni/src/GLShaderProgram.cpp \
    ../../pineapple/jni/src/GLPrimitive.cpp \
    ../../pineapple/jni/src/GLFramebufferObject.cpp \
    ../../pineapple/jni/src/VSML.cpp \
    ../../pineapple/jni/src/GL.cpp \
    ../../pineapple/jni/src/GLTexture.cpp \
    ../../pineapple/jni/src/Engine.cpp \
    ../jni/src/Simfile.cpp \
    ../jni/src/PockyState.cpp \
    ../jni/src/PockyGame.cpp
HEADERS  += mainwindow.h \
    glwidget.h \
    ../../pineapple/jni/extern/GLShaderProgram.h \
    ../../pineapple/jni/extern/GLPrimitive.h \
    ../../pineapple/jni/extern/GLFramebufferObject.h \
    ../../pineapple/jni/extern/VSML.h \
    ../../pineapple/jni/extern/GL.h \
    ../../pineapple/jni/extern/GLTexture.h \
    ../../pineapple/jni/extern/Engine.h \
    ../jni/include/Simfile.h \
    ../jni/include/PockyState.h \
    ../jni/include/PockyGame.h \
    ../../pineapple/jni/extern/Common.h
FORMS    += mainwindow.ui

QMAKE_CXXFLAGS += -std=c++0x
