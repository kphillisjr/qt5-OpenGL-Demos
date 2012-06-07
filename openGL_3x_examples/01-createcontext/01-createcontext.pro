HEADERS       = glwidget.h \
                window.h 
SOURCES       = glwidget.cpp \
                main.cpp \
                window.cpp 
QT           += opengl widgets

# install
target.path = $$[QT_INSTALL_EXAMPLES]/qt_OpenGL_3x/01_Creating_OpenGL_Context
sources.files = $$SOURCES $$HEADERS $$RESOURCES $$FORMS 01-createcontext.pro
sources.path = $$[QT_INSTALL_EXAMPLES]/qt_OpenGL_3x/hellogl
INSTALLS += target sources


simulator: warning(This example might not fully work on Simulator platform)
