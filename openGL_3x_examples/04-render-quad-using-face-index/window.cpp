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
#include <QLabel>
#include "glwidget.h"
#include "window.h"

//! [0]
Window::Window()
{
    glWidget = new GLWidget;
    QGLFormat base_format = glWidget->format();
    // Remove Deprecated Functions.
    base_format.setProfile(QGLFormat::CoreProfile);
    glWidget->setFormat(base_format);

    // Connect the signal for the boolean click.
    //connect(glWidget, SIGNAL(forgotToBindChanged(bool)), glWidget, SLOT(forgotToBind(bool)));
//! [0]
	glShaderStatus = new QLabel;

//! [1]
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(glWidget);
    mainLayout->addWidget(glShaderStatus);
    setLayout(mainLayout);

    setWindowTitle(tr("OpenGL 3.x - Rendering Quads using Indices"));
    // Make Sure the Default value for shader is enabled.
    ForgotShaderBind = false;    
    glWidget->forgotToBindShader(ForgotShaderBind);
    UpdateLabel();
}
//! [1]
void Window::UpdateLabel()
{
	if(glShaderStatus)
	{
		if(ForgotShaderBind)
			glShaderStatus->setText("Shader Is Currently Not Set.");
		else
			glShaderStatus->setText("You Remembered to Set the Shader.");
	}
}
void Window::mousePressEvent(QMouseEvent *event)
{
 	if(event->type () & QEvent::MouseButtonPress){
		if (event->buttons() & Qt::RightButton) {
			ForgotShaderBind=!ForgotShaderBind;
		    glWidget->forgotToBindShader(ForgotShaderBind);
	        UpdateLabel();
		    return;
    	}
    }
	    QWidget::mousePressEvent(event);	
}

void Window::keyPressEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_Escape) {
        close();
    } else if (e->key() == Qt::Key_Space) {
	    ForgotShaderBind=!ForgotShaderBind;
        glWidget->forgotToBindShader(ForgotShaderBind);
        UpdateLabel();
    } else {
        QWidget::keyPressEvent(e);
    }
}
