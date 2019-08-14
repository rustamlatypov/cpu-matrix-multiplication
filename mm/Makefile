bin=cp-test cp-benchmark main
include ../common/Makefile.common

SOURCES+=./../cp-common/*.cc ./../cp-common/*.ccp ./../common/*.cc
CXXFLAGS+=-I ./ -I ./../cp-common -I ./../common -fopenmp
LDFLAGS+=-fopenmp
vpath %.h ../cp-common:../common
vpath %.cc ../cp-common:../common

main: main.o cp.o
	$(CXX) $(LDFLAGS) $^ -o $@

main.o: main.cpp
	$(CXX) $(LDFLAGS) $^ -c $@

cp-test: cp-test.o cp.o error.o
	$(CXX) $(LDFLAGS) $^ -o $@

cp-benchmark: cp-benchmark.o cp.o error.o
	$(CXX) $(LDFLAGS) $^ -o $@

include Makefile.dep