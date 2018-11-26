#ifndef MAINOPENGLWIDGET_H
#define MAINOPENGLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QSurfaceFormat>
#include <QOpenGLShaderProgram>
#include "QOpenGLFunctions_3_2_Core"
#include <QMatrix4x4>
#include <QDebug>
#include <QFileInfo>
#include <mesh.h>

class MainOpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{

public:
    QMatrix4x4 m_projection;
    MainOpenGLWidget(QWidget *parent);

    Mesh const& getMesh() const;
    void setMesh(Mesh const& p_mesh);

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    Mesh m_mesh;

    GLuint m_posAttr;
    GLuint m_colAttr;
    GLuint m_normalAttr;
    GLuint m_matrixUniform;
    QOpenGLShaderProgram *m_program;
};

#endif // MAINOPENGLWIDGET_H
