bin=test benchmark run
include ../common/Makefile.common

SOURCES+= ./../scripts/*.cc ./../common/*.cc 
CXXFLAGS+=-I ./ -I ./../common -fopenmp
LDFLAGS+=-fopenmp

run: run.o
	$(CXX) $(LDFLAGS) $^ -o $@

test: test.o 
	$(CXX) $(LDFLAGS) $^ -o $@

benchmark: benchmark.o 
	$(CXX) $(LDFLAGS) $^ -o $@