#include "glwidget.h"
#include <pocky/jni/include/PockyGame.h>
#include <pocky/jni/include/PockyState.h>
#include <pineapple/jni/extern/Engine.h>
#include <QGLWidget>
#include <QTimer>
#include <QTime>
#include <QMouseEvent>
#include <QDebug>
using namespace Pocky;
using namespace Pineapple;

Pocky::PockyGame *g_Game;
PockyState *g_State;
QTimer *g_Timer;
GLWidget::GLWidget(QWidget *parent) : QGLWidget(parent)
{

	Engine::init();
	GL::init();
	PockyGameParams parms;
	parms.gridx = 5;
	parms.gridy = 3;
	g_Game = new Pocky::PockyGame(parms);
	g_State = new PockyState(g_Game);
	Engine::instance()->setUpdatable(g_State);
	Engine::instance()->start();
	this->setAutoBufferSwap(false);
	g_Timer = new QTimer();
	g_Timer->setInterval(1000/ 30);
	QObject::connect(g_Timer, SIGNAL(timeout()), this, SLOT(update()));
	g_Timer->start();


}

void GLWidget::initializeGL() {
	GL::instance()->createShader("texmap", "assets/shaders/texmap.glsl");
	GL::instance()->createShader("default", "assets/shaders/default.glsl");

	GL::instance()->initializeGL(this->width(), this->height());
	g_Game->init();

}

void GLWidget::paintGL()
{

	//g_State->update();
	//qDebug("tick");
//	glClearColor(0.f, 0.f, 0.f, 0.f);
//	glClear(GL_COLOR_BUFFER_BIT);
	g_Game->draw(QTime::currentTime().msec());

	this->swapBuffers();
}

void GLWidget::mousePressEvent(QMouseEvent *event) {
	qDebug() << "ID:::" << g_Game->getGridLocation(event->x(), event->y());

	g_State->touch(event->x(), event->y());
}

void GLWidget::mouseMoveEvent(QMouseEvent *event) {

}

void GLWidget::mouseReleaseEvent(QMouseEvent *event) {

}

