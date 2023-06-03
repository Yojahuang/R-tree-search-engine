CXX = g++
CFLAGS = -std=c++14 -O3 -Wall -shared -fPIC `python3-config --includes` 
CIMPORTPATH = "includes"

all: RTree.so

RTree.so: includes/Utils.hpp includes/RTree.h includes/Node.h includes/Geometry.h srcs/Geometry.cpp srcs/RTree.cpp pybind.cpp  
	$(CXX) $^ $(CFLAGS) -I/usr/include/mk -I${CIMPORTPATH} -o $@

clean:
	rm -f *.so

test:
	python3 -m pytest tests/RTree_test.py -v

.PHONY: all clean test