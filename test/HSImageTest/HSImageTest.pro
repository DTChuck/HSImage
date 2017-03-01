TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle

QMAKE_CXXFLAGS += -std=c++11

SOURCES += main.cpp

LIBS += -lHSImage

#OpenCV Includes and Libs
LIBS += -L/usr/local/lib \
        -lopencv_core \
        -lopencv_highgui \
        -lopencv_imgproc \
        -lopencv_imgcodecs
