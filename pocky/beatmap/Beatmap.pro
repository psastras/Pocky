# -------------------------------------------------
# Project created by QtCreator 2011-12-08T13:58:20
# -------------------------------------------------
QT += core \
    gui \
    opengl
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
    ../jni/src/PockyGame.cpp \
    ../../pineapple/jni/src/Audio.cpp
HEADERS += mainwindow.h \
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
    ../../pineapple/jni/extern/Common.h \
    ../../pineapple/jni/extern/Audio.h
FORMS += mainwindow.ui
LIBS += -lvorbis \
    -lvorbisfile \
    -logg \
    -L/home/psastras/Downloads/openal-soft-1.13 \
    -lopenal
QMAKE_CXXFLAGS += -std=c++0x
OTHER_FILES += ../assets/shaders/text.glsl \
    ../assets/shaders/texmaplit.glsl \
    ../assets/shaders/texmap.glsl \
    ../assets/shaders/overlay.glsl \
    ../assets/shaders/id.glsl \
    ../assets/shaders/hex.glsl \
    ../assets/shaders/default2.glsl \
    ../assets/shaders/default.glsl \
    ../assets/shaders/blur.glsl \
    ../assets/shaders/bloom.glsl \
    ../assets/shaders/background.glsl \
    ../assets/shaders/alpha.glsl \
    ../assets/shaders/touch.glsl
