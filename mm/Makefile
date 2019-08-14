bin=test benchmark run
include ../common/Makefile.common

SOURCES+= ./../common/*.cc ./../mm/*.cc
CXXFLAGS+=-I ./ -I ./../common -fopenmp
LDFLAGS+=-fopenmp
vpath %.h ../cp-common:../common
vpath %.cc ../cp-common:../common

run: run.o
	$(CXX) $(LDFLAGS) $^ -o $@

test: test.o cp.o error.o
	$(CXX) $(LDFLAGS) $^ -o $@

benchmark: benchmark.o cp.o error.o
	$(CXX) $(LDFLAGS) $^ -o $@

include Makefile.dep