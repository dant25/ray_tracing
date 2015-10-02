#-------------------------------------------------
#
# Project created by QtCreator 2015-09-24T14:00:52
#
#-------------------------------------------------

QT       += core gui

QMAKE_CXXFLAGS+= -fopenmp
QMAKE_LFLAGS +=  -fopenmp


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Imagem
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    Camera.cpp \
    Sphere.cpp \
    math/Mat4x4.cpp \
    math/Vec3.cpp \
    Primitive.cpp \
    Material.cpp \
    Color.cpp \
    scene.cpp \
    math/point3d.cpp

HEADERS  += mainwindow.h \
    Camera.h \
    Ray.h \
    Sphere.h \
    Intersect.h \
    math/Mat4x4.h \
    math/Point.h \
    math/Vec3.h \
    Primitive.h \
    Material.h \
    Color.h \
    BoundingBox.h \
    scene.h \
    math/Quaternions.h \
    math/point3d.h

FORMS    += mainwindow.ui
