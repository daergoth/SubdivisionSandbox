#include "MainOpenGLWidget.h"
#include "QOpenGLFunctions_3_2_Core"


MainOpenGLWidget::MainOpenGLWidget(QWidget *parent) : QOpenGLWidget(parent)
{
    setFormat(QSurfaceFormat::defaultFormat());
}

static const char *vertexShaderSource =
        "#version 330\n"
        "in vec4 posAttr;\n"
        "in vec4 colAttr;\n"
        "out vec4 col;\n"
        "uniform mat4 matrix;\n"
        "void main()\n"
        "{\n"
        //"   col = vec4(0,0,1,1);\n"
        "     col = colAttr;\n"
        "   gl_Position = matrix*posAttr;\n"
        "}\n";

static const char *fragmentShaderSource =
        "#version 330\n"
        "in vec4 col;\n"
        "out vec4 fragcol;\n"
        "void main()\n"
        "{\n"
        "   fragcol = col;\n"
        "}\n";

void MainOpenGLWidget::initializeGL()
{
    makeCurrent();
    initializeOpenGLFunctions();
    glClearColor(0.7f, 0.76f, 0.88f, 1.0f);

    // Create shader
    m_program = new QOpenGLShaderProgram(this);
    m_program -> addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource);
    m_program -> addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource);
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

    /*GLfloat vertices[] =
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
    glDisableVertexAttribArray(m_colAttr);*/

    m_program -> release();
}

void MainOpenGLWidget::drawCubeObject(){
    glClearColor(0.7f, 0.76f, 0.88f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    makeCurrent();

    static const GLfloat quadVertices[] ={
      0.5f,-0.5f,-4.0f,1.0f,
        0.5f,0.5f,-4.0f,1.0f,
        -0.5f,0.5f,-4.0f,1.0f,
        -0.5f,-0.5f,-4.0f,1.0f,

        0.5f,-0.5f,-4.0f,1.0f,
        0.5f,-0.5f,-2.0f,1.0f,
        0.5f,0.5f,-2.0f,1.0f,
        0.5f,0.5f,-4.0f,1.0f,

        -0.5f,-0.5f,-4.0f,1.0f,
        -0.5f,-0.5f,-2.0f,1.0f,
        -0.5f,0.5f,-2.0f,1.0f,
        -0.5f,0.5f,-4.0f,1.0f,

        0.5f,0.5f,-4.0f,1.0f,
        0.5f,0.5f,-2.0f,1.0f,
        -0.5f,0.5f,-2.0f,1.0f,
        -0.5f,0.5f,-4.0f,1.0f,

        0.5f,-0.5f,-4.0f,1.0f,
        0.5f,-0.5f,-2.0f,1.0f,
        -0.5f,-0.5f,-2.0f,1.0f,
        -0.5f,-0.5f,-4.0f,1.0f

       /* 0.5f,0.5f,-2.0f,1.0f,
          0.5f,-0.5f,-2.0f,1.0f,
          -0.5f,-0.5f,-2.0f,1.0f,
          -0.5f,0.5f,-2.0f,1.0f*/
    };

    GLfloat colors[] =
    {
        0.583f,  0.771f,  0.014f,
            0.609f,  0.115f,  0.436f,
            0.327f,  0.483f,  0.844f,
            0.822f,  0.569f,  0.201f,
            0.435f,  0.602f,  0.223f,
            0.310f,  0.747f,  0.185f,
            0.597f,  0.770f,  0.761f,
            0.559f,  0.436f,  0.730f,
            0.359f,  0.583f,  0.152f,
            0.483f,  0.596f,  0.789f,
            0.559f,  0.861f,  0.639f,
            0.195f,  0.548f,  0.859f,
            0.014f,  0.184f,  0.576f,
            0.771f,  0.328f,  0.970f,
            0.406f,  0.615f,  0.116f,
            0.676f,  0.977f,  0.133f,
            0.971f,  0.572f,  0.833f,
            0.140f,  0.616f,  0.489f,
            0.997f,  0.513f,  0.064f,
            0.945f,  0.719f,  0.592f,
            0.543f,  0.021f,  0.978f,
            0.279f,  0.317f,  0.505f,
            0.167f,  0.620f,  0.077f,
            0.347f,  0.857f,  0.137f,
            0.055f,  0.953f,  0.042f,
            0.714f,  0.505f,  0.345f,
            0.783f,  0.290f,  0.734f,
            0.722f,  0.645f,  0.174f,
            0.302f,  0.455f,  0.848f,
            0.225f,  0.587f,  0.040f,
            0.517f,  0.713f,  0.338f,
            0.053f,  0.959f,  0.120f,
            0.393f,  0.621f,  0.362f,
            0.673f,  0.211f,  0.457f,
            0.820f,  0.883f,  0.371f,
            0.982f,  0.099f,  0.879f
    };

    m_program -> bind();
    m_program -> setUniformValue(m_matrixUniform, m_projection);

    glVertexAttribPointer(m_posAttr, 4, GL_FLOAT, GL_FALSE, 0, quadVertices);
    glVertexAttribPointer(m_colAttr, 3, GL_FLOAT, GL_FALSE, 0, colors);

    glEnableVertexAttribArray(m_posAttr);
    glEnableVertexAttribArray(m_colAttr);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glDrawArrays(GL_QUADS,0,4*6);

    glDisableVertexAttribArray(m_posAttr);
    glDisableVertexAttribArray(m_colAttr);

    m_program -> release();
}


