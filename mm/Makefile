bin=test benchmark main
include ../common/Makefile.common

SOURCES+=./../cp-common/*.cc ./../common/*.cc ./../mm/*.cc
CXXFLAGS+=-I ./ -I ./../cp-common -I ./../common -fopenmp
LDFLAGS+=-fopenmp
vpath %.h ../cp-common:../common
vpath %.cc ../cp-common:../common

main: main.o
	$(CXX) $(LDFLAGS) $^ -o $@

test: test.o cp.o error.o
	$(CXX) $(LDFLAGS) $^ -o $@

benchmark: benchmark.o cp.o error.o
	$(CXX) $(LDFLAGS) $^ -o $@

include Makefile.dep