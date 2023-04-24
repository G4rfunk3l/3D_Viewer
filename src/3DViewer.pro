# Ubuntu
linux-g++ {
    message("Building on Linux")

    QT += core
    QT += gui
    QT += opengl
}

# macOS
macx {
    message("Building on macOS")

    QT += core
    QT += gui
    QT += opengl
    QT += openglwidgets
}

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
include(3rdparty/qtgifimage/src/gifimage/qtgifimage.pri)

SOURCES += \
    3DViewer.c \
    glwidget.cpp \
    main.cpp \
    mainwindow.cpp \
    oglgeometry.cpp

HEADERS += \
    3DViewer.h \
    glwidget.h \
    mainwindow.h \
    oglgeometry.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \

    3rdparty/qtgifimage/src/gifimage/qtgifimage.pri \

DEFINES += SRCDIR=\\\"$$PWD/\\\"

RESOURCES += \
    shader.qrc

