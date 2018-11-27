#ifndef MAINOPENGLWIDGET_H
#define MAINOPENGLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QSurfaceFormat>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions_3_2_Core>
#include <QMatrix4x4>
#include <QDebug>
#include <QFileInfo>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>


class MainOpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{

public:
    QMatrix4x4 m_projection;
    MainOpenGLWidget(QWidget *parent);

protected:
    void initializeGL() override;

    void resizeGL(int w, int h) override;

    void paintGL() override;

    GLuint m_posAttr;
    GLuint m_colAttr;
    GLuint m_matrixUniform;
    QOpenGLShaderProgram *m_program;

};

#endif // MAINOPENGLWIDGET_H
