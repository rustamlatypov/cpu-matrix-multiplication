bin=test benchmark run help
include Makefile.common

SOURCES+= ./../scripts/*.cc 
CXXFLAGS+=-I ./../common -fopenmp -O2 -std=c++17
LDFLAGS+=-fopenmp

run: run.o
	$(CXX) $(LDFLAGS) $^ -o $@

test: test.o 
	$(CXX) $(LDFLAGS) $^ -o $@

benchmark: benchmark.o 
	$(CXX) $(LDFLAGS) $^ -o $@

help: help.o 
	$(CXX) $(LDFLAGS) $^ -o $@