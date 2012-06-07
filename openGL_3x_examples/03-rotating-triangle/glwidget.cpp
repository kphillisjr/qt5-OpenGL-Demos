/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Nokia Corporation and its Subsidiary(-ies) nor
**     the names of its contributors may be used to endorse or promote
**     products derived from this software without specific prior written
**     permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtWidgets>
#include <QtOpenGL>

#include <math.h>

#include "glwidget.h"
//#include "qtlogo.h"

#ifndef GL_MULTISAMPLE
#define GL_MULTISAMPLE  0x809D
#endif

//! [0]
GLWidget::GLWidget(QWidget *parent)
    : QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{
    xRot = 0;
    yRot = 0;
    qtGreen = QColor::fromCmykF(0.40, 0.0, 1.0, 0.0);
    qtPurple = QColor::fromCmykF(0.39, 0.39, 0.0, 0.0);
    mforgotToBindShader = false;
}
//! [0]

//! [1]
GLWidget::~GLWidget()
{
}
//! [1]

//! [2]
QSize GLWidget::minimumSizeHint() const
{
    return QSize(50, 50);
}
//! [2]

//! [3]
QSize GLWidget::sizeHint() const
//! [3] //! [4]
{
    return QSize(400, 400);
}
//! [4]

void GLWidget::forgotToBindShader(bool value)
{
 	if(value != mforgotToBindShader)
 	{
		mforgotToBindShader = value;
		emit forgotToBindShaderChanged(mforgotToBindShader);
		updateGL();
    }
}
//! [6]
void GLWidget::initializeGL()
{
    qglClearColor(qtPurple.dark());
	mforgotToBindShader = false;

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_MULTISAMPLE);
    static GLfloat lightPosition[4] = { 0.5, 5.0, 7.0, 1.0 };
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
    
    QGLShader *vshader1 = new QGLShader(QGLShader::Vertex, this);
	const char *vsrc1 = 
#ifdef QT_OPENGL_ES_2
    	"#version 100\n"  // OpenGL ES 2.0
#else
    	"#version 120\n"  // OpenGL 2.1
#endif
    	"attribute vec2 coord2d;                  \n"
    	"uniform mat4 m_transform;                \n"
	    "void main(void) {                        \n"
    	"  gl_Position = m_transform * vec4(coord2d, 0.0, 1.0); \n"
	    "}\n";
    vshader1->compileSourceCode(vsrc1);

    QGLShader *fshader1 = new QGLShader(QGLShader::Fragment, this);
    const char *fsrc1 =
#ifdef QT_OPENGL_ES_2
    	"#version 100\n"  // OpenGL ES 2.0
#else
    	"#version 120\n"  // OpenGL 2.1
#endif
		"void main(void) {        \n"
		"  gl_FragColor[0] = 0.0; \n"
		"  gl_FragColor[1] = 0.0; \n"
		"  gl_FragColor[2] = 1.0; \n"
		"}\n";
    fshader1->compileSourceCode(fsrc1);

    program1.addShader(vshader1);
    program1.addShader(fshader1);
    program1.link();
    vertexAttr1 = program1.attributeLocation("coord2d");
	matrixUniform1 = program1.uniformLocation( "m_transform");
    // Create the vertex buffer.
    vertices.clear();
    vertices << QVector2D(0.0f,0.8f);
    vertices << QVector2D(-0.8f,-0.8f);
    vertices << QVector2D(0.8f,-0.8f);
    RotationMatrix.setToIdentity();
}



static void qNormalizeAngle(int &angle)
{
    while (angle < 0)
        angle += 360 * 16;
    while (angle > 360 * 16)
        angle -= 360 * 16;
}

//! [5]
void GLWidget::setXRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != xRot) {
        xRot = angle;
        RotationMatrix.rotate ( angle, 0.0f, 0.0f, 0.5f );
        emit xRotationChanged(angle);
        updateGL();
    }
}
//! [5]


void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - lastPos.x();
    int dy = event->y() - lastPos.y();

    if (event->buttons() & Qt::LeftButton) {
        setXRotation(xRot + 0.2 * dy);
    }
    lastPos = event->pos();
}


void GLWidget::setYRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != yRot) {
        yRot = angle;
        emit yRotationChanged(angle);
        updateGL();
    }
}

//! [6]
void GLWidget::resetRotationMatrix()
{
    RotationMatrix.setToIdentity();
}

//! [7]
void GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    // Do not forget to Bind Shader. If you forget, You will not see anything.
	if(mforgotToBindShader == false)
	{
	    program1.bind();
    }
    program1.enableAttributeArray(vertexAttr1);
    program1.setAttributeArray(vertexAttr1, vertices.constData());
    program1.setUniformValue(matrixUniform1,RotationMatrix);
    glDrawArrays(GL_TRIANGLES, 0, vertices.size());
    program1.disableAttributeArray(vertexAttr1);
	if(mforgotToBindShader == false)
	{
	    program1.release();
    }
}
//! [7]

//! [8]
void GLWidget::resizeGL(int width, int height)
{
    int side = qMin(width, height);
    glViewport((width - side) / 2, (height - side) / 2, side, side);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
}
//! [8]
