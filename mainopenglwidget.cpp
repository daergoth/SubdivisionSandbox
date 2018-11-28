#include "MainOpenGLWidget.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>


MainOpenGLWidget::MainOpenGLWidget(QWidget *parent) : QOpenGLWidget(parent)
{
    setFormat(QSurfaceFormat::defaultFormat());

    //m_mesh = Mesh::makeTriangle();
    m_mesh = Mesh::makeCube();

    m_mesh = Mesh(m_mesh.convertToSurfaceMesh());

    m_eye = QVector3D(0,0,2);
    m_forward = QVector3D(0.0f,0.0f,-1.0f);

    setMouseTracking(true);

    m_width = width();
    m_height = height();

    update();
}

Mesh const& MainOpenGLWidget::getMesh() const
{
    return m_mesh;
}

void MainOpenGLWidget::setMesh(Mesh const& p_mesh)
{
    m_mesh = p_mesh;
}

void MainOpenGLWidget::update()
{
    m_projection.setToIdentity();
    m_projection.perspective(60.0f, m_width / float(m_height), 0.01f, 100.0f);
    m_view.setToIdentity();
    m_view.lookAt(m_eye, m_eye+m_forward, QVector3D(0,1,0));

    QOpenGLWidget::update();
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
    m_normalAttr = m_program -> attributeLocation("normalAttr");

    m_matrixUniform = m_program -> uniformLocation("matrix");
    m_tintUniform = m_program -> uniformLocation("tint");


    m_program -> release();
}

void MainOpenGLWidget::resizeGL(int w, int h)
{
    m_width = w;
    m_height = h;
    glViewport(0,0, w, h);
    update();
}

void MainOpenGLWidget::paintGL()
{
    // Draw the scene:
    QOpenGLFunctions_3_2_Core *f = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_2_Core>();
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    makeCurrent();

    m_program -> bind();
    m_program -> setUniformValue(m_matrixUniform, m_projection*m_view);

    glVertexAttribPointer(m_posAttr, 3, GL_FLOAT, GL_FALSE, 9*sizeof(float), (float*)(m_mesh.m_vertices.data()));
    glVertexAttribPointer(m_normalAttr, 3, GL_FLOAT, GL_FALSE, 9*sizeof(float), (float*)(m_mesh.m_vertices.data())+3);
    glVertexAttribPointer(m_colAttr, 3, GL_FLOAT, GL_FALSE, 9*sizeof(float), (float*)(m_mesh.m_vertices.data())+6);


    glEnableVertexAttribArray(m_posAttr);
    glEnableVertexAttribArray(m_colAttr);

    glEnable(GL_DEPTH_TEST);

    m_program->setUniformValue(m_tintUniform, 1.0f,1.0f,1.0f,1.0f);
    glDrawElements(GL_TRIANGLES, m_mesh.m_indices.size(), GL_UNSIGNED_INT, m_mesh.m_indices.data());

    m_program->setUniformValue(m_tintUniform, 0.0f,0.0f,0.0f,1.0f);
    glLineWidth(5.0f);
    f->glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glEnable(GL_POLYGON_OFFSET_LINE);
    glPolygonOffset(-1.0f,0.0f);
    glDrawElements(GL_TRIANGLES, m_mesh.m_indices.size(), GL_UNSIGNED_INT, m_mesh.m_indices.data());
    f->glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDisable(GL_POLYGON_OFFSET_LINE);

    glDisableVertexAttribArray(m_posAttr);
    glDisableVertexAttribArray(m_normalAttr);
    glDisableVertexAttribArray(m_colAttr);

    m_program -> release();
}

void MainOpenGLWidget::mousePressEvent(QMouseEvent *event)
{}

void MainOpenGLWidget::mouseReleaseEvent(QMouseEvent *event)
{}

void MainOpenGLWidget::mouseMoveEvent(QMouseEvent *event)
{
    // Delta coordinates
    int dx = event->x() - m_mouseX, dy = event->y() - m_mouseY;

    // Whether this is the first call or not
    bool firstCall = m_mouseX == -1 && m_mouseY == -1;

    // Store the new mouse coordinates
    m_mouseX = event->x();
    m_mouseY = event->y();

    // Skip any logic if this was the first call
    if (firstCall) return;

    QVector3D right = QVector3D::crossProduct(QVector3D(0,1,0), m_forward).normalized();
    QVector3D up = QVector3D::crossProduct(m_forward,right).normalized();

    // Rotate the camera
    if ((event->buttons() & Qt::LeftButton) != 0)
    {
        static const float X_SCALE = 1.0f / 50.0f * qDegreesToRadians(180.0f);
        static const float Y_SCALE = 1.0f / 50.0f * qDegreesToRadians(180.0f);

        float scale = qMax(1.0f, (1.0f + m_eye.length()) / 1000.0f);

        QMatrix4x4 rotateX, rotateY;
        rotateX.rotate(dy * X_SCALE * scale, right);
        rotateY.rotate(-dx * Y_SCALE * scale, up);
        QMatrix4x4 rotate = rotateX * rotateY;

        m_forward = QVector3D(rotate*QVector4D(m_forward,0.0f)).normalized();

        update();
    }

    // Move the camera
    else if ((event->buttons() & Qt::RightButton) != 0)
    {
        float scale = qMax(0.01f, (1.0f + m_eye.length()) / 3200.0f);
        m_eye += dx * scale * right;
        m_eye += dy * scale * up;

        update();
    }
}

QSize MainOpenGLWidget::sizeHint() const
{
    return QSize(800, 600);
}