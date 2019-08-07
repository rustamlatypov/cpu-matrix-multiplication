bin=cp-test cp-benchmark
include ../common/Makefile.common

SOURCES+=./../cp-common/*.cc ./../common/*.cc
CXXFLAGS+=-I ./ -I ./../cp-common -I ./../common -fopenmp
LDFLAGS+=-fopenmp
vpath %.h ../cp-common:../common
vpath %.cc ../cp-common:../common

mm: cp.o 
	$(CXX) $(LDFLAGS) $^ -o $@

cp-test: cp-test.o cp.o error.o
	$(CXX) $(LDFLAGS) $^ -o $@

cp-benchmark: cp-benchmark.o cp.o error.o
	$(CXX) $(LDFLAGS) $^ -o $@

include Makefile.dep