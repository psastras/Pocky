#include <QtGui/QApplication>
#include "mainwindow.h"
#include "../../pineapple/jni/extern/Engine.h"
#include "../../pineapple/jni/extern/GL.h"
Pineapple::Engine *Pineapple::Engine::s_instance = 0;
Pineapple::GL *Pineapple::GL::s_instance = 0;
int main(int argc, char *argv[])
{


    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
