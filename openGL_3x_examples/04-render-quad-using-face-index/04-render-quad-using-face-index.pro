HEADERS       = glwidget.h \
                window.h 
SOURCES       = glwidget.cpp \
                main.cpp \
                window.cpp 
QT           += opengl widgets

# install
target.path = $$[QT_INSTALL_EXAMPLES]/qt_OpenGL_3x/02_First_Triangle
sources.files = $$SOURCES $$HEADERS $$RESOURCES $$FORMS 02-first-triangle.pro
sources.path = $$[QT_INSTALL_EXAMPLES]/qt_OpenGL_3x/02_First_Triangle
INSTALLS += target sources


simulator: warning(This example might not fully work on Simulator platform)
