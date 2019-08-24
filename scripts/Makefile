bin=test benchmark run 
include Makefile.common

SOURCES+= ./../scripts/*.cc 
CXXFLAGS+=-I ./../common -fopenmp -mno-fused-madd
LDFLAGS+=-fopenmp

run: run.o
	$(CXX) $(LDFLAGS) $^ -o $@

test: test.o 
	$(CXX) $(LDFLAGS) $^ -o $@

benchmark: benchmark.o 
	$(CXX) $(LDFLAGS) $^ -o $@