# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

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
CMAKE_SOURCE_DIR = /home/parallels/Desktop/GitHub_Work/MQP/RS_Proj/examples

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/parallels/Desktop/GitHub_Work/MQP/RS_Proj/examples/hello-realsense

# Include any dependencies generated for this target.
include hello-realsense/CMakeFiles/rs-hello-realsense.dir/depend.make

# Include the progress variables for this target.
include hello-realsense/CMakeFiles/rs-hello-realsense.dir/progress.make

# Include the compile flags for this target's objects.
include hello-realsense/CMakeFiles/rs-hello-realsense.dir/flags.make

hello-realsense/CMakeFiles/rs-hello-realsense.dir/rs-hello-realsense.cpp.o: hello-realsense/CMakeFiles/rs-hello-realsense.dir/flags.make
hello-realsense/CMakeFiles/rs-hello-realsense.dir/rs-hello-realsense.cpp.o: rs-hello-realsense.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/parallels/Desktop/GitHub_Work/MQP/RS_Proj/examples/hello-realsense/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object hello-realsense/CMakeFiles/rs-hello-realsense.dir/rs-hello-realsense.cpp.o"
	cd /home/parallels/Desktop/GitHub_Work/MQP/RS_Proj/examples/hello-realsense/hello-realsense && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/rs-hello-realsense.dir/rs-hello-realsense.cpp.o -c /home/parallels/Desktop/GitHub_Work/MQP/RS_Proj/examples/hello-realsense/rs-hello-realsense.cpp

hello-realsense/CMakeFiles/rs-hello-realsense.dir/rs-hello-realsense.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/rs-hello-realsense.dir/rs-hello-realsense.cpp.i"
	cd /home/parallels/Desktop/GitHub_Work/MQP/RS_Proj/examples/hello-realsense/hello-realsense && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/parallels/Desktop/GitHub_Work/MQP/RS_Proj/examples/hello-realsense/rs-hello-realsense.cpp > CMakeFiles/rs-hello-realsense.dir/rs-hello-realsense.cpp.i

hello-realsense/CMakeFiles/rs-hello-realsense.dir/rs-hello-realsense.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/rs-hello-realsense.dir/rs-hello-realsense.cpp.s"
	cd /home/parallels/Desktop/GitHub_Work/MQP/RS_Proj/examples/hello-realsense/hello-realsense && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/parallels/Desktop/GitHub_Work/MQP/RS_Proj/examples/hello-realsense/rs-hello-realsense.cpp -o CMakeFiles/rs-hello-realsense.dir/rs-hello-realsense.cpp.s

# Object files for target rs-hello-realsense
rs__hello__realsense_OBJECTS = \
"CMakeFiles/rs-hello-realsense.dir/rs-hello-realsense.cpp.o"

# External object files for target rs-hello-realsense
rs__hello__realsense_EXTERNAL_OBJECTS =

hello-realsense/rs-hello-realsense: hello-realsense/CMakeFiles/rs-hello-realsense.dir/rs-hello-realsense.cpp.o
hello-realsense/rs-hello-realsense: hello-realsense/CMakeFiles/rs-hello-realsense.dir/build.make
hello-realsense/rs-hello-realsense: hello-realsense/CMakeFiles/rs-hello-realsense.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/parallels/Desktop/GitHub_Work/MQP/RS_Proj/examples/hello-realsense/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable rs-hello-realsense"
	cd /home/parallels/Desktop/GitHub_Work/MQP/RS_Proj/examples/hello-realsense/hello-realsense && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/rs-hello-realsense.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
hello-realsense/CMakeFiles/rs-hello-realsense.dir/build: hello-realsense/rs-hello-realsense

.PHONY : hello-realsense/CMakeFiles/rs-hello-realsense.dir/build

hello-realsense/CMakeFiles/rs-hello-realsense.dir/clean:
	cd /home/parallels/Desktop/GitHub_Work/MQP/RS_Proj/examples/hello-realsense/hello-realsense && $(CMAKE_COMMAND) -P CMakeFiles/rs-hello-realsense.dir/cmake_clean.cmake
.PHONY : hello-realsense/CMakeFiles/rs-hello-realsense.dir/clean

hello-realsense/CMakeFiles/rs-hello-realsense.dir/depend:
	cd /home/parallels/Desktop/GitHub_Work/MQP/RS_Proj/examples/hello-realsense && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/parallels/Desktop/GitHub_Work/MQP/RS_Proj/examples /home/parallels/Desktop/GitHub_Work/MQP/RS_Proj/examples/hello-realsense /home/parallels/Desktop/GitHub_Work/MQP/RS_Proj/examples/hello-realsense /home/parallels/Desktop/GitHub_Work/MQP/RS_Proj/examples/hello-realsense/hello-realsense /home/parallels/Desktop/GitHub_Work/MQP/RS_Proj/examples/hello-realsense/hello-realsense/CMakeFiles/rs-hello-realsense.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : hello-realsense/CMakeFiles/rs-hello-realsense.dir/depend

