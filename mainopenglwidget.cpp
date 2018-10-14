#include "MainOpenGLWidget.h"

void MainOpenGLWidget::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
}

void MainOpenGLWidget::resizeGL(int w, int h)
{
    // Update projection matrix and other size related settings:
    m_projection.setToIdentity();
    m_projection.perspective(45.0f, w / float(h), 0.01f, 100.0f);
}

void MainOpenGLWidget::paintGL()
{
    // Draw the scene:
    f->glClear(GL_COLOR_BUFFER_BIT);
}
