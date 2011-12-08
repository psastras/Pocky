#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>

class GLWidget : public QGLWidget
{
public:
	GLWidget(QWidget *parent = 0);




protected:
	void mouseMoveEvent ( QMouseEvent * event );
	void mousePressEvent ( QMouseEvent * event );
	void mouseReleaseEvent ( QMouseEvent * event );
	void initializeGL();
	void paintGL();
	void update();

};

#endif // GLWIDGET_H
