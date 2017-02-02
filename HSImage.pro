#-------------------------------------------------
#
# Project created by QtCreator 2015-03-18T13:16:04
#
#-------------------------------------------------

QT       -= core gui

TARGET = HSImage
TEMPLATE = lib

DEFINES += HSIMAGE_LIBRARY

SOURCES += hsimage.cpp \
    classifiedhsimage.cpp \
    target.cpp \
    colormap.cpp \
    main.cpp

HEADERS += hsimage.h\
        hsimage_global.h \
        classifiedhsimage.h \
        annEnums.h \
        target.h \
        colormap.h

QMAKE_CXXFLAGS += -std=c++11

unix {

CONFIG += ryan_computer #comment this line out to run on josh's computer

ryan_computer {
    INCLUDEPATH += /usr/include/python3.4
} else {
    INCLUDEPATH += /usr/include/python3.5
}

INCLUDEPATH += /usr/include/boost \

LIBS += -L/usr/local/lib \
        -lopencv_core \
        -lopencv_highgui \
        -lopencv_imgproc \
        -lopencv_imgcodecs \
        -lboost_system \
        -lboost_python3 \

ryan_computer {
    LIBS += -L/usr/lib/python3.4/config-3.4m-x86_64-linux-gnu \
        -lpython3.4
} else {
    LIBS += -L/usr/lib/python3.5/config-3.5m-x86_64-linux-gnu \
        -lpython3.5
}


    target.path = /usr/local/lib
    INSTALLS += target

    headers.path = /usr/local/include
    headers.files = $$HEADERS

    INSTALLS += headers

    QMAKE_POST_LINK = cp libHSImage.so.1.0.0 HSI.so
}
