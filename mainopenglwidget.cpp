#include "MainOpenGLWidget.h"
#include "QOpenGLFunctions_3_2_Core"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>


MainOpenGLWidget::MainOpenGLWidget(QWidget *parent) : QOpenGLWidget(parent)
{
    setFormat(QSurfaceFormat::defaultFormat());

    m_mesh = Mesh::makeTriangle();
}

Mesh const& MainOpenGLWidget::getMesh() const
{
    return m_mesh;
}

void MainOpenGLWidget::setMesh(Mesh const& p_mesh)
{
    m_mesh = p_mesh;
}

void MainOpenGLWidget::initializeGL()
{
    makeCurrent();
    initializeOpenGLFunctions();
    glClearColor(0.7f, 0.76f, 0.88f, 1.0f);

    // Create shader
    m_program = new QOpenGLShaderProgram(this);
    m_program -> addShaderFromSourceFile(QOpenGLShader::Vertex, "D:\\Egyetem\\MSc\\Geometriai modellezes\\Project\\SubdivisionSandbox\\shaders\\vertexshader.vert");
    m_program -> addShaderFromSourceFile(QOpenGLShader::Fragment, "D:\\Egyetem\\MSc\\Geometriai modellezes\\Project\\SubdivisionSandbox\\shaders\\fragmentshader.frag");
    m_program -> link();
    m_posAttr = m_program -> attributeLocation("posAttr");
    m_colAttr = m_program -> attributeLocation("colAttr");
    m_normalAttr = m_program -> attributeLocation("normalAttr");
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

    glVertexAttribPointer(m_posAttr, 3, GL_FLOAT, GL_FALSE, 9*sizeof(float), (float*)(m_mesh.m_vertices.data()));
    glVertexAttribPointer(m_normalAttr, 3, GL_FLOAT, GL_FALSE, 9*sizeof(float), (float*)(m_mesh.m_vertices.data())+3);
    glVertexAttribPointer(m_colAttr, 3, GL_FLOAT, GL_FALSE, 9*sizeof(float), (float*)(m_mesh.m_vertices.data())+6);


    glEnableVertexAttribArray(m_posAttr);
    glEnableVertexAttribArray(m_colAttr);

    glDisable(GL_DEPTH_TEST);

    glDrawElements(GL_TRIANGLES, m_mesh.m_indices.size(), GL_UNSIGNED_INT, m_mesh.m_indices.data());

    glDisableVertexAttribArray(m_posAttr);
    glDisableVertexAttribArray(m_normalAttr);
    glDisableVertexAttribArray(m_colAttr);

    m_program -> release();
}
