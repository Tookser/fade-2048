# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/scow/Desktop/2048_template/src

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/scow/Desktop/2048_template/src

# Include any dependencies generated for this target.
include bin/CMakeFiles/2048.dir/depend.make

# Include the progress variables for this target.
include bin/CMakeFiles/2048.dir/progress.make

# Include the compile flags for this target's objects.
include bin/CMakeFiles/2048.dir/flags.make

bin/CMakeFiles/2048.dir/main.cpp.o: bin/CMakeFiles/2048.dir/flags.make
bin/CMakeFiles/2048.dir/main.cpp.o: bin/main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/scow/Desktop/2048_template/src/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object bin/CMakeFiles/2048.dir/main.cpp.o"
	cd /home/scow/Desktop/2048_template/src/bin && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/2048.dir/main.cpp.o -c /home/scow/Desktop/2048_template/src/bin/main.cpp

bin/CMakeFiles/2048.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/2048.dir/main.cpp.i"
	cd /home/scow/Desktop/2048_template/src/bin && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/scow/Desktop/2048_template/src/bin/main.cpp > CMakeFiles/2048.dir/main.cpp.i

bin/CMakeFiles/2048.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/2048.dir/main.cpp.s"
	cd /home/scow/Desktop/2048_template/src/bin && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/scow/Desktop/2048_template/src/bin/main.cpp -o CMakeFiles/2048.dir/main.cpp.s

bin/CMakeFiles/2048.dir/main.cpp.o.requires:

.PHONY : bin/CMakeFiles/2048.dir/main.cpp.o.requires

bin/CMakeFiles/2048.dir/main.cpp.o.provides: bin/CMakeFiles/2048.dir/main.cpp.o.requires
	$(MAKE) -f bin/CMakeFiles/2048.dir/build.make bin/CMakeFiles/2048.dir/main.cpp.o.provides.build
.PHONY : bin/CMakeFiles/2048.dir/main.cpp.o.provides

bin/CMakeFiles/2048.dir/main.cpp.o.provides.build: bin/CMakeFiles/2048.dir/main.cpp.o


# Object files for target 2048
2048_OBJECTS = \
"CMakeFiles/2048.dir/main.cpp.o"

# External object files for target 2048
2048_EXTERNAL_OBJECTS =

bin/2048: bin/CMakeFiles/2048.dir/main.cpp.o
bin/2048: bin/CMakeFiles/2048.dir/build.make
bin/2048: display/libdisplay_lib.a
bin/2048: engine/libengine_lib.a
bin/2048: motor/libmotor_lib.a
bin/2048: display/libdisplay_lib.a
bin/2048: /usr/lib/x86_64-linux-gnu/libGL.so
bin/2048: /usr/lib/x86_64-linux-gnu/libglfw.so.3.2
bin/2048: engine/libengine_lib.a
bin/2048: bin/CMakeFiles/2048.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/scow/Desktop/2048_template/src/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable 2048"
	cd /home/scow/Desktop/2048_template/src/bin && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/2048.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
bin/CMakeFiles/2048.dir/build: bin/2048

.PHONY : bin/CMakeFiles/2048.dir/build

bin/CMakeFiles/2048.dir/requires: bin/CMakeFiles/2048.dir/main.cpp.o.requires

.PHONY : bin/CMakeFiles/2048.dir/requires

bin/CMakeFiles/2048.dir/clean:
	cd /home/scow/Desktop/2048_template/src/bin && $(CMAKE_COMMAND) -P CMakeFiles/2048.dir/cmake_clean.cmake
.PHONY : bin/CMakeFiles/2048.dir/clean

bin/CMakeFiles/2048.dir/depend:
	cd /home/scow/Desktop/2048_template/src && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/scow/Desktop/2048_template/src /home/scow/Desktop/2048_template/src/bin /home/scow/Desktop/2048_template/src /home/scow/Desktop/2048_template/src/bin /home/scow/Desktop/2048_template/src/bin/CMakeFiles/2048.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : bin/CMakeFiles/2048.dir/depend

