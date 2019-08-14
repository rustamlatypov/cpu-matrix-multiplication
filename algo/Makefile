bin=run test benchmark
include ../common/Makefile.common

SOURCES+=./../common/*.cc ./../algo/*.cc
CXXFLAGS+=-I ./ -I ./../cp-common -I ./../common -fopenmp
LDFLAGS+=-fopenmp

run: run.o
	$(CXX) $(LDFLAGS) $^ -o $@

test: test.o fast.o error.o
	$(CXX) $(LDFLAGS) $^ -o $@

benchmark: benchmark.o fast.o error.o
	$(CXX) $(LDFLAGS) $^ -o $@

include Makefile.dep