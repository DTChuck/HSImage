#-------------------------------------------------
#
# Project created by QtCreator 2015-03-18T13:16:04
#
#-------------------------------------------------

QT       -= gui

TARGET = HSImage
TEMPLATE = lib

DEFINES += HSIMAGE_LIBRARY

SOURCES += hsimage.cpp \
    classifiedhsimage.cpp \
    target.cpp \
    colormap.cpp

HEADERS += hsimage.h\
        hsimage_global.h \
        classifiedhsimage.h \
        annEnums.h \
        target.h \
        colormap.h

QMAKE_CXXFLAGS += -std=c++11

unix {
#OpenCV Includes and Libs
INCLUDEPATH += /usr/include/python3.4 \
INCLUDEPATH =+ /usr/include/boost_1_63_0 \


LIBS += -L/usr/local/lib \
        -lopencv_core \
        -lopencv_highgui \
        -lopencv_imgproc \
        -lopencv_imgcodecs \
        -lboost_system \
        -lboost_python3 \

LIBS += -L/usr/lib/python3.4/config-3.4m-x86_64-linux-gnu \
        -lpython3.4


    target.path = /usr/local/lib
    INSTALLS += target

    headers.path = /usr/local/include
    headers.files = $$HEADERS

    INSTALLS += headers
}
