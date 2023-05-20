CXX = g++
CFLAGS = -std=c++14 -O3 -Wall -shared -fPIC `python3-config --includes` 
CIMPORTPATH = "includes"

all: rtree.so

rtree.so: includes/RTree.h 
	$(CXX) $^ $(CFLAGS) -I/usr/include/mk -I${CIMPORTPATH} -o $@

clean:
	rm -f *.so

test:
	python3 -m pytest tests/RTree_test.py -v

.PHONY: all clean test