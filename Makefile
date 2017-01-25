# location of the Python header files
 
PYTHON_VERSION = 3.4
PYTHON_INCLUDE = /usr/include/python$(PYTHON_VERSION)
 
# location of the Boost Python include files and library
 
BOOST_INC = /usr/include
BOOST_LIB = /usr/lib
 
# compile mesh classes
##TARGET= Interaction_Class

.PHONY: all

all: classifiedhsimage.so colormap.so hsimage.so target.so
 
greeting.so: greeting.o gesture.o
	g++ -shared -Wl,--export-dynamic greeting.o gesture.o -L$(BOOST_LIB) -lboost_python3 -L/usr/lib/python$(PYTHON_VERSION)/config-3.4m-x86_64-linux-gnu -lpython$(PYTHON_VERSION) -o greeting.so # Binaries

gesture.so: gesture.o
	g++ -shared -Wl,--export-dynamic gesture.o -L$(BOOST_LIB) -lboost_python3 -L/usr/lib/python$(PYTHON_VERSION)/config-3.4m-x86_64-linux-gnu -lpython$(PYTHON_VERSION) -o gesture.so # Binaries
 

classifiedhsimage.so: classifiedhsimage.o colormap.o hsimage.o 

colormap.so: colormap.o

hsimage.so: hsimage.o

target.so: target.o


classifiedhsimage.o: classifiedhsimage.cpp
	g++ -I$(PYTHON_INCLUDE) -I$(BOOST_INC) -fPIC -c classifiedhsimage.cpp # Headers

colormap.o: colormap.cpp
	g++ -I$(PYTHON_INCLUDE) -I$(BOOST_INC) -fPIC -c colormap.cpp

hsimage.o: hsimage.cpp
	g++ -I$(PYTHON_INCLUDE) -I$(BOOST_INC) -fPIC -c hsimage.cpp

target.o: target.cpp
	g++ -I$(PYTHON_INCLUDE) -I$(BOOST_INC) -fPIC -c target.cpp

