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
#include <mesh.h>
#include <QMouseEvent>
#include <QtMath>

class MainOpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{

public:
    QMatrix4x4 m_projection;
    MainOpenGLWidget(QWidget *parent);

    Mesh const& getMesh() const;
    void setMesh(Mesh const& p_mesh);

    QSize sizeHint() const Q_DECL_OVERRIDE;

protected:
    void update();
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

    Mesh m_mesh;

    GLuint m_posAttr;
    GLuint m_colAttr;
    GLuint m_normalAttr;
    GLuint m_matrixUniform;
    GLuint m_tintUniform;
    QOpenGLShaderProgram *m_program;

    QVector3D m_eye;
    QVector3D m_forward;
    QMatrix4x4 m_view;

    int m_mouseX = -1;
    int m_mouseY = -1;

    int m_width, m_height;

};

#endif // MAINOPENGLWIDGET_H
