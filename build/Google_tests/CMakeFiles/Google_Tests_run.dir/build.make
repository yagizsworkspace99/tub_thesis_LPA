# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.28

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
CMAKE_COMMAND = /opt/homebrew/Cellar/cmake/3.28.0/bin/cmake

# The command to remove a file.
RM = /opt/homebrew/Cellar/cmake/3.28.0/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/yagizsemercioglu/Desktop/tempus-framework

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/yagizsemercioglu/Desktop/tempus-framework/build

# Include any dependencies generated for this target.
include Google_tests/CMakeFiles/Google_Tests_run.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include Google_tests/CMakeFiles/Google_Tests_run.dir/compiler_depend.make

# Include the progress variables for this target.
include Google_tests/CMakeFiles/Google_Tests_run.dir/progress.make

# Include the compile flags for this target's objects.
include Google_tests/CMakeFiles/Google_Tests_run.dir/flags.make

Google_tests/CMakeFiles/Google_Tests_run.dir/test1.cpp.o: Google_tests/CMakeFiles/Google_Tests_run.dir/flags.make
Google_tests/CMakeFiles/Google_Tests_run.dir/test1.cpp.o: /Users/yagizsemercioglu/Desktop/tempus-framework/Google_tests/test1.cpp
Google_tests/CMakeFiles/Google_Tests_run.dir/test1.cpp.o: Google_tests/CMakeFiles/Google_Tests_run.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/yagizsemercioglu/Desktop/tempus-framework/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object Google_tests/CMakeFiles/Google_Tests_run.dir/test1.cpp.o"
	cd /Users/yagizsemercioglu/Desktop/tempus-framework/build/Google_tests && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT Google_tests/CMakeFiles/Google_Tests_run.dir/test1.cpp.o -MF CMakeFiles/Google_Tests_run.dir/test1.cpp.o.d -o CMakeFiles/Google_Tests_run.dir/test1.cpp.o -c /Users/yagizsemercioglu/Desktop/tempus-framework/Google_tests/test1.cpp

Google_tests/CMakeFiles/Google_Tests_run.dir/test1.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/Google_Tests_run.dir/test1.cpp.i"
	cd /Users/yagizsemercioglu/Desktop/tempus-framework/build/Google_tests && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/yagizsemercioglu/Desktop/tempus-framework/Google_tests/test1.cpp > CMakeFiles/Google_Tests_run.dir/test1.cpp.i

Google_tests/CMakeFiles/Google_Tests_run.dir/test1.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/Google_Tests_run.dir/test1.cpp.s"
	cd /Users/yagizsemercioglu/Desktop/tempus-framework/build/Google_tests && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/yagizsemercioglu/Desktop/tempus-framework/Google_tests/test1.cpp -o CMakeFiles/Google_Tests_run.dir/test1.cpp.s

# Object files for target Google_Tests_run
Google_Tests_run_OBJECTS = \
"CMakeFiles/Google_Tests_run.dir/test1.cpp.o"

# External object files for target Google_Tests_run
Google_Tests_run_EXTERNAL_OBJECTS =

Google_tests/Google_Tests_run: Google_tests/CMakeFiles/Google_Tests_run.dir/test1.cpp.o
Google_tests/Google_Tests_run: Google_tests/CMakeFiles/Google_Tests_run.dir/build.make
Google_tests/Google_Tests_run: lib/libgtest.a
Google_tests/Google_Tests_run: lib/libgtest_main.a
Google_tests/Google_Tests_run: lib/libgtest.a
Google_tests/Google_Tests_run: lib/libgtest_main.a
Google_tests/Google_Tests_run: lib/libgtest.a
Google_tests/Google_Tests_run: Google_tests/CMakeFiles/Google_Tests_run.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/Users/yagizsemercioglu/Desktop/tempus-framework/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable Google_Tests_run"
	cd /Users/yagizsemercioglu/Desktop/tempus-framework/build/Google_tests && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Google_Tests_run.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
Google_tests/CMakeFiles/Google_Tests_run.dir/build: Google_tests/Google_Tests_run
.PHONY : Google_tests/CMakeFiles/Google_Tests_run.dir/build

Google_tests/CMakeFiles/Google_Tests_run.dir/clean:
	cd /Users/yagizsemercioglu/Desktop/tempus-framework/build/Google_tests && $(CMAKE_COMMAND) -P CMakeFiles/Google_Tests_run.dir/cmake_clean.cmake
.PHONY : Google_tests/CMakeFiles/Google_Tests_run.dir/clean

Google_tests/CMakeFiles/Google_Tests_run.dir/depend:
	cd /Users/yagizsemercioglu/Desktop/tempus-framework/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/yagizsemercioglu/Desktop/tempus-framework /Users/yagizsemercioglu/Desktop/tempus-framework/Google_tests /Users/yagizsemercioglu/Desktop/tempus-framework/build /Users/yagizsemercioglu/Desktop/tempus-framework/build/Google_tests /Users/yagizsemercioglu/Desktop/tempus-framework/build/Google_tests/CMakeFiles/Google_Tests_run.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : Google_tests/CMakeFiles/Google_Tests_run.dir/depend

