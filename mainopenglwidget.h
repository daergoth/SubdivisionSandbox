#ifndef MAINOPENGLWIDGET_H
#define MAINOPENGLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>

class MainOpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{

public:
    MainOpenGLWidget(QWidget *parent) : QOpenGLWidget(parent) { }

protected:
    void initializeGL() override;

    void resizeGL(int w, int h) override;

    void paintGL() override;

};

#endif // MAINOPENGLWIDGET_H
