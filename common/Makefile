bin=pngdiff
include ../common/Makefile.common

pngdiff: pngdiff.o pngio.o 
	$(CXX) $(LDFLAGS) $^ -lpng -o $@

