# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
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
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /n/eecs583a/home/ahmedaj/DO_check_and_optimization/Backend/DOFplicm

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /n/eecs583a/home/ahmedaj/DO_check_and_optimization/Backend/DOFplicm/build

# Include any dependencies generated for this target.
include DOFplicm/CMakeFiles/LLVMDOFplicm.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include DOFplicm/CMakeFiles/LLVMDOFplicm.dir/compiler_depend.make

# Include the progress variables for this target.
include DOFplicm/CMakeFiles/LLVMDOFplicm.dir/progress.make

# Include the compile flags for this target's objects.
include DOFplicm/CMakeFiles/LLVMDOFplicm.dir/flags.make

DOFplicm/CMakeFiles/LLVMDOFplicm.dir/DOFplicm.cpp.o: DOFplicm/CMakeFiles/LLVMDOFplicm.dir/flags.make
DOFplicm/CMakeFiles/LLVMDOFplicm.dir/DOFplicm.cpp.o: ../DOFplicm/DOFplicm.cpp
DOFplicm/CMakeFiles/LLVMDOFplicm.dir/DOFplicm.cpp.o: DOFplicm/CMakeFiles/LLVMDOFplicm.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/n/eecs583a/home/ahmedaj/DO_check_and_optimization/Backend/DOFplicm/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object DOFplicm/CMakeFiles/LLVMDOFplicm.dir/DOFplicm.cpp.o"
	cd /n/eecs583a/home/ahmedaj/DO_check_and_optimization/Backend/DOFplicm/build/DOFplicm && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT DOFplicm/CMakeFiles/LLVMDOFplicm.dir/DOFplicm.cpp.o -MF CMakeFiles/LLVMDOFplicm.dir/DOFplicm.cpp.o.d -o CMakeFiles/LLVMDOFplicm.dir/DOFplicm.cpp.o -c /n/eecs583a/home/ahmedaj/DO_check_and_optimization/Backend/DOFplicm/DOFplicm/DOFplicm.cpp

DOFplicm/CMakeFiles/LLVMDOFplicm.dir/DOFplicm.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/LLVMDOFplicm.dir/DOFplicm.cpp.i"
	cd /n/eecs583a/home/ahmedaj/DO_check_and_optimization/Backend/DOFplicm/build/DOFplicm && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /n/eecs583a/home/ahmedaj/DO_check_and_optimization/Backend/DOFplicm/DOFplicm/DOFplicm.cpp > CMakeFiles/LLVMDOFplicm.dir/DOFplicm.cpp.i

DOFplicm/CMakeFiles/LLVMDOFplicm.dir/DOFplicm.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/LLVMDOFplicm.dir/DOFplicm.cpp.s"
	cd /n/eecs583a/home/ahmedaj/DO_check_and_optimization/Backend/DOFplicm/build/DOFplicm && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /n/eecs583a/home/ahmedaj/DO_check_and_optimization/Backend/DOFplicm/DOFplicm/DOFplicm.cpp -o CMakeFiles/LLVMDOFplicm.dir/DOFplicm.cpp.s

# Object files for target LLVMDOFplicm
LLVMDOFplicm_OBJECTS = \
"CMakeFiles/LLVMDOFplicm.dir/DOFplicm.cpp.o"

# External object files for target LLVMDOFplicm
LLVMDOFplicm_EXTERNAL_OBJECTS =

DOFplicm/LLVMDOFplicm.so: DOFplicm/CMakeFiles/LLVMDOFplicm.dir/DOFplicm.cpp.o
DOFplicm/LLVMDOFplicm.so: DOFplicm/CMakeFiles/LLVMDOFplicm.dir/build.make
DOFplicm/LLVMDOFplicm.so: DOFplicm/CMakeFiles/LLVMDOFplicm.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/n/eecs583a/home/ahmedaj/DO_check_and_optimization/Backend/DOFplicm/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX shared module LLVMDOFplicm.so"
	cd /n/eecs583a/home/ahmedaj/DO_check_and_optimization/Backend/DOFplicm/build/DOFplicm && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/LLVMDOFplicm.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
DOFplicm/CMakeFiles/LLVMDOFplicm.dir/build: DOFplicm/LLVMDOFplicm.so
.PHONY : DOFplicm/CMakeFiles/LLVMDOFplicm.dir/build

DOFplicm/CMakeFiles/LLVMDOFplicm.dir/clean:
	cd /n/eecs583a/home/ahmedaj/DO_check_and_optimization/Backend/DOFplicm/build/DOFplicm && $(CMAKE_COMMAND) -P CMakeFiles/LLVMDOFplicm.dir/cmake_clean.cmake
.PHONY : DOFplicm/CMakeFiles/LLVMDOFplicm.dir/clean

DOFplicm/CMakeFiles/LLVMDOFplicm.dir/depend:
	cd /n/eecs583a/home/ahmedaj/DO_check_and_optimization/Backend/DOFplicm/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /n/eecs583a/home/ahmedaj/DO_check_and_optimization/Backend/DOFplicm /n/eecs583a/home/ahmedaj/DO_check_and_optimization/Backend/DOFplicm/DOFplicm /n/eecs583a/home/ahmedaj/DO_check_and_optimization/Backend/DOFplicm/build /n/eecs583a/home/ahmedaj/DO_check_and_optimization/Backend/DOFplicm/build/DOFplicm /n/eecs583a/home/ahmedaj/DO_check_and_optimization/Backend/DOFplicm/build/DOFplicm/CMakeFiles/LLVMDOFplicm.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : DOFplicm/CMakeFiles/LLVMDOFplicm.dir/depend

