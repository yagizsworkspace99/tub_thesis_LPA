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
CMAKE_BINARY_DIR = /Users/yagizsemercioglu/Desktop/tempus-framework/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/MyTest.dir/depend.make
# Include the progress variables for this target.
include CMakeFiles/MyTest.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/MyTest.dir/flags.make

CMakeFiles/MyTest.dir/test.cpp.o: CMakeFiles/MyTest.dir/flags.make
CMakeFiles/MyTest.dir/test.cpp.o: /Users/yagizsemercioglu/Desktop/tempus-framework/test.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/yagizsemercioglu/Desktop/tempus-framework/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/MyTest.dir/test.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/MyTest.dir/test.cpp.o -c /Users/yagizsemercioglu/Desktop/tempus-framework/test.cpp

CMakeFiles/MyTest.dir/test.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/MyTest.dir/test.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/yagizsemercioglu/Desktop/tempus-framework/test.cpp > CMakeFiles/MyTest.dir/test.cpp.i

CMakeFiles/MyTest.dir/test.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/MyTest.dir/test.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/yagizsemercioglu/Desktop/tempus-framework/test.cpp -o CMakeFiles/MyTest.dir/test.cpp.s

# Object files for target MyTest
MyTest_OBJECTS = \
"CMakeFiles/MyTest.dir/test.cpp.o"

# External object files for target MyTest
MyTest_EXTERNAL_OBJECTS =

MyTest: CMakeFiles/MyTest.dir/test.cpp.o
MyTest: CMakeFiles/MyTest.dir/build.make
MyTest: lib/libgtest.a
MyTest: lib/libgtest_main.a
MyTest: lib/libgtest.a
MyTest: CMakeFiles/MyTest.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/Users/yagizsemercioglu/Desktop/tempus-framework/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable MyTest"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/MyTest.dir/link.txt --verbose=$(VERBOSE)
	/opt/homebrew/Cellar/cmake/3.28.0/bin/cmake -D TEST_TARGET=MyTest -D TEST_EXECUTABLE=/Users/yagizsemercioglu/Desktop/tempus-framework/cmake-build-debug/MyTest -D TEST_EXECUTOR= -D TEST_WORKING_DIR=/Users/yagizsemercioglu/Desktop/tempus-framework/cmake-build-debug -D TEST_EXTRA_ARGS= -D TEST_PROPERTIES= -D TEST_PREFIX= -D TEST_SUFFIX= -D TEST_FILTER= -D NO_PRETTY_TYPES=FALSE -D NO_PRETTY_VALUES=FALSE -D TEST_LIST=MyTest_TESTS -D CTEST_FILE=/Users/yagizsemercioglu/Desktop/tempus-framework/cmake-build-debug/MyTest[1]_tests.cmake -D TEST_DISCOVERY_TIMEOUT=5 -D TEST_XML_OUTPUT_DIR= -P /opt/homebrew/Cellar/cmake/3.28.0/share/cmake/Modules/GoogleTestAddTests.cmake

# Rule to build all files generated by this target.
CMakeFiles/MyTest.dir/build: MyTest
.PHONY : CMakeFiles/MyTest.dir/build

CMakeFiles/MyTest.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/MyTest.dir/cmake_clean.cmake
.PHONY : CMakeFiles/MyTest.dir/clean

CMakeFiles/MyTest.dir/depend:
	cd /Users/yagizsemercioglu/Desktop/tempus-framework/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/yagizsemercioglu/Desktop/tempus-framework /Users/yagizsemercioglu/Desktop/tempus-framework /Users/yagizsemercioglu/Desktop/tempus-framework/cmake-build-debug /Users/yagizsemercioglu/Desktop/tempus-framework/cmake-build-debug /Users/yagizsemercioglu/Desktop/tempus-framework/cmake-build-debug/CMakeFiles/MyTest.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/MyTest.dir/depend

