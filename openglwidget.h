#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H
#include <QtOpenGLWidgets/QOpenGLWidget>
//#include <QOpenGLWidget>
#include <QOpenGLFunctions_4_5_Core>

class OpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions_4_5_Core
{
public:
    OpenGLWidget(QWidget *parent = 0);

protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

};

#endif
