CXX = g++
CC = $(CXX)
RM = rm -f

DEBUG_LEVEL = -g
CPPFLAGS = -c -Wall -std=c++0x -Woverflow -Wextra 

LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
DIRECTORIES = $(wildcard */)
DIRINCLUDE = $(addprefix -I , $(DIRECTORIES))
SOURCES = $(wildcard **/*.hpp) $(wildcard *.cpp)
OBJECTS = $(subst .cpp,.o,$(wildcard **/*.cpp)) $(subst .cpp,.o,$(wildcard *.cpp))

EXECUTABLE = main

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@

.cpp.o:
	$(CC) $(CPPFLAGS) $< -o $@ $(DIRINCLUDE)

clean:
	$(RM) $(OBJECTS) $(EXECUTABLE)

#"make" to compile
#"make clean" to remove .o-files
#"./main" to execute program