#-------------------------------------------------
#
# Project created by QtCreator 2018-10-01T13:03:08
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SubdivisionSandbox
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
    main.cpp \
    MainWindow.cpp \
    Mesh.cpp \
    MainOpenGLWidget.cpp \
    SubdivisionController.cpp \
    LoopSubdivisionStrategy.cpp \
    ButterflySubdivisionStrategy.cpp \
    CatmullClarkSubdivisionStrategy.cpp \
    KobbeltSubdivisionStrategy.cpp \
    CustomSchemeSubdivisionStrategy.cpp \
    CustomScheme.cpp \
    CustomSchemeHandler.cpp \
    CustomSchemeWindow.cpp

HEADERS += \
    CustomScheme.h \
    CustomSchemeHandler.h \
    MainOpenGLWidget.h \
    MainWindow.h \
    Mesh.h \
    SubdivisionController.h \
    SubdivisionStrategy.h \
    LoopSubdivisionStrategy.h \
    ButterflySubdivisionStrategy.h \
    CatmullClarkSubdivisionStrategy.h \
    KobbeltSubdivisionStrategy.h \
    CustomSchemeSubdivisionStrategy.h \
    CustomSchemeWindow.h

FORMS += \
    MainWindow.ui \
    CustomSchemeWindow.ui

#INCLUDEPATH  += $$PWD/3rdparty/assimp/include
#LIBS += -L$$PWD/3rdparty/assimp/lib/ -lassimp

# Assimp
win32:CONFIG(release, debug|release): LIBS += -L$$PWD/3rdparty/assimp/lib/ -lassimp
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/3rdparty/assimp/lib/ -lassimpd
else:unix: LIBS += -L$$PWD/3rdparty/assimp/lib/ -lassimp


INCLUDEPATH += $$PWD/3rdparty/assimp/include
DEPENDPATH += $$PWD/3rdparty/assimp/include

# Boost
INCLUDEPATH  += $$PWD/3rdparty/boost/include

# GMP
INCLUDEPATH  += $$PWD/3rdparty/gmp/include/
LIBS += -L$$PWD/3rdparty/gmp/lib/ -llibgmp-10

# MPFR
INCLUDEPATH  += $$PWD/3rdparty/mpfr/include/
LIBS += -L$$PWD/3rdparty/mpfr/lib/ -llibmpfr-4

# CGAL
INCLUDEPATH  += $$PWD/3rdparty/CGAL/include
CONFIG(debug, debug|release): LIBS += -L$$PWD/3rdparty/CGAL/lib/ -lCGAL_Core-vc140-mt-gd-4.10 -lCGAL-vc140-mt-gd-4.10
CONFIG(release, debug|release): LIBS += -L$$PWD/3rdparty/CGAL/lib/ -lCGAL_Core-vc140-mt-4.10 -lCGAL-vc140-mt-4.10

# nlohmann/json
INCLUDEPATH += $$PWD/3rdparty/json/include
DEPENDPATH += $$PWD/3rdparty/json/include

RESOURCES += \
    resources.qrc

