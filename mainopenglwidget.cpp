#include "MainOpenGLWidget.h"
#include "QOpenGLFunctions_3_2_Core"


MainOpenGLWidget::MainOpenGLWidget(QWidget *parent) : QOpenGLWidget(parent)
{
    setFormat(QSurfaceFormat::defaultFormat());
}

void MainOpenGLWidget::initializeGL()
{
    makeCurrent();
    initializeOpenGLFunctions();
    glClearColor(0.7f, 0.76f, 0.88f, 1.0f);

    // Create shader
    m_program = new QOpenGLShaderProgram(this);
    m_program -> addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/vertex");
    m_program -> addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/fragment");
    m_program -> link();
    m_posAttr = m_program -> attributeLocation("posAttr");
    m_colAttr = m_program -> attributeLocation("colAttr");
    m_matrixUniform = m_program -> uniformLocation("matrix");

    m_program -> release();
}

void MainOpenGLWidget::resizeGL(int w, int h)
{
    // Update projection matrix and other size related settings:
    m_projection.setToIdentity();
    m_projection.perspective(60.0f, w / float(h), 0.01f, 100.0f);
    //m_projection.translate(0,0,-2);

    glViewport(0,0, w, h);
}

void MainOpenGLWidget::paintGL()
{
    // Draw the scene:
    //QOpenGLFunctions_3_2_Core *f = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_2_Core>();
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    makeCurrent();

    GLfloat vertices[] =
    {
        0.0f, 0.707f, -2.0f, 1.0f,
        -0.5f, -0.5f, -2.0f, 1.0f,
        0.5f, -0.5f, -2.0f, 1.0f
    };

    GLfloat colors[] =
    {
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f
    };

    m_program -> bind();
    m_program -> setUniformValue(m_matrixUniform, m_projection);

    glVertexAttribPointer(m_posAttr, 4, GL_FLOAT, GL_FALSE, 0, vertices);
    glVertexAttribPointer(m_colAttr, 3, GL_FLOAT, GL_FALSE, 0, colors);

    glEnableVertexAttribArray(m_posAttr);
    glEnableVertexAttribArray(m_colAttr);

    glDisable(GL_DEPTH_TEST);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    glDisableVertexAttribArray(m_posAttr);
    glDisableVertexAttribArray(m_colAttr);

    m_program -> release();
}
