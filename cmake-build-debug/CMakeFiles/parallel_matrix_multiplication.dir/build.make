# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.14

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /cygdrive/c/Users/rulat/.CLion2019.1/system/cygwin_cmake/bin/cmake.exe

# The command to remove a file.
RM = /cygdrive/c/Users/rulat/.CLion2019.1/system/cygwin_cmake/bin/cmake.exe -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /cygdrive/c/Users/rulat/parallel-matrix-multiplication

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /cygdrive/c/Users/rulat/parallel-matrix-multiplication/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/parallel_matrix_multiplication.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/parallel_matrix_multiplication.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/parallel_matrix_multiplication.dir/flags.make

CMakeFiles/parallel_matrix_multiplication.dir/mm/main.cpp.o: CMakeFiles/parallel_matrix_multiplication.dir/flags.make
CMakeFiles/parallel_matrix_multiplication.dir/mm/main.cpp.o: ../mm/main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/cygdrive/c/Users/rulat/parallel-matrix-multiplication/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/parallel_matrix_multiplication.dir/mm/main.cpp.o"
	/usr/bin/c++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/parallel_matrix_multiplication.dir/mm/main.cpp.o -c /cygdrive/c/Users/rulat/parallel-matrix-multiplication/mm/main.cpp

CMakeFiles/parallel_matrix_multiplication.dir/mm/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/parallel_matrix_multiplication.dir/mm/main.cpp.i"
	/usr/bin/c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /cygdrive/c/Users/rulat/parallel-matrix-multiplication/mm/main.cpp > CMakeFiles/parallel_matrix_multiplication.dir/mm/main.cpp.i

CMakeFiles/parallel_matrix_multiplication.dir/mm/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/parallel_matrix_multiplication.dir/mm/main.cpp.s"
	/usr/bin/c++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /cygdrive/c/Users/rulat/parallel-matrix-multiplication/mm/main.cpp -o CMakeFiles/parallel_matrix_multiplication.dir/mm/main.cpp.s

# Object files for target parallel_matrix_multiplication
parallel_matrix_multiplication_OBJECTS = \
"CMakeFiles/parallel_matrix_multiplication.dir/mm/main.cpp.o"

# External object files for target parallel_matrix_multiplication
parallel_matrix_multiplication_EXTERNAL_OBJECTS =

parallel_matrix_multiplication.exe: CMakeFiles/parallel_matrix_multiplication.dir/mm/main.cpp.o
parallel_matrix_multiplication.exe: CMakeFiles/parallel_matrix_multiplication.dir/build.make
parallel_matrix_multiplication.exe: CMakeFiles/parallel_matrix_multiplication.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/cygdrive/c/Users/rulat/parallel-matrix-multiplication/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable parallel_matrix_multiplication.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/parallel_matrix_multiplication.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/parallel_matrix_multiplication.dir/build: parallel_matrix_multiplication.exe

.PHONY : CMakeFiles/parallel_matrix_multiplication.dir/build

CMakeFiles/parallel_matrix_multiplication.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/parallel_matrix_multiplication.dir/cmake_clean.cmake
.PHONY : CMakeFiles/parallel_matrix_multiplication.dir/clean

CMakeFiles/parallel_matrix_multiplication.dir/depend:
	cd /cygdrive/c/Users/rulat/parallel-matrix-multiplication/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /cygdrive/c/Users/rulat/parallel-matrix-multiplication /cygdrive/c/Users/rulat/parallel-matrix-multiplication /cygdrive/c/Users/rulat/parallel-matrix-multiplication/cmake-build-debug /cygdrive/c/Users/rulat/parallel-matrix-multiplication/cmake-build-debug /cygdrive/c/Users/rulat/parallel-matrix-multiplication/cmake-build-debug/CMakeFiles/parallel_matrix_multiplication.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/parallel_matrix_multiplication.dir/depend

