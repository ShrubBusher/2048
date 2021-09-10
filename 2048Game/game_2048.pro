QT += core gui widgets opengl openglwidgets

LIBS += -lopengl32 -lglu32

TARGET = 2048

TEMPLATE = app



SOURCES += \
    glwidget.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    glwidget.h \
    main.h \
    mainwindow.h \

FORMS += \
    mainwindow.ui


RESOURCES += \
    textures.qrc
