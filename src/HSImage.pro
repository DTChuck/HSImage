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
    main.cpp \
    python_utils.cpp \
    #pyboost_cv3_converter.cpp \
    labelfile.cpp \
    jsoncpp/jsoncpp.cpp \
    pybind11_opencv_numpy/ndarray_converter.cpp

HEADERS += hsimage.h\
    #hsimage_global.h \
    classifiedhsimage.h \
    annEnums.h \
    target.h \
    colormap.h \
    python_utils.h \
    #pyboostconverter/pyboostcvconverter.hpp \
    labelfile.h \
    jsoncpp/json/json.h \
    jsoncpp/json/json-forwards.h \
    pybind11_opencv_numpy/ndarray_converter.h

QMAKE_CXXFLAGS += -std=c++11

unix {

CONFIG += ryan_computer#comment this line out when not on Ryans's computer
#CONFIG += perception_computer #comment this line out when not on perception computer
ryan_computer {
    INCLUDEPATH += /usr/include/python3.4
    INCLUDEPATH += /home/ryan/.virtualenvs/cv/lib/python3.4/site-packages/numpy/core/include
} else:perception_computer {
    INCLUDEPATH += /usr/include/python3.4
    INCLUDEPATH += /home/lab/.virtualenvs/ml/lib/python3.4/site-packages/numpy/core/include
} else {
    INCLUDEPATH += /usr/include/python3.5
    INCLUDEPATH += /home/joshua/.virtualenvs/cv/lib/python3.5/site-packages/numpy/core/include
}

INCLUDEPATH += /usr/include/boost \

LIBS += -L/usr/local/lib \
        -lopencv_core \
        -lopencv_highgui \
        -lopencv_imgproc \
        -lopencv_imgcodecs \
        -lboost_system \


}
ryan_computer {
    LIBS += -L/usr/lib/python3.4/config-3.4m-x86_64-linux-gnu \
        -lpython3.4
} else:perception_computer{
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
#}
