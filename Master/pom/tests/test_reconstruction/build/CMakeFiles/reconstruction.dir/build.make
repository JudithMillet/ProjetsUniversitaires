# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.23

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
CMAKE_SOURCE_DIR = /home/judith/Documents/M1/POM/pom-s2/tests/test_reconstruction

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/judith/Documents/M1/POM/pom-s2/tests/test_reconstruction/build

# Include any dependencies generated for this target.
include CMakeFiles/reconstruction.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/reconstruction.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/reconstruction.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/reconstruction.dir/flags.make

CMakeFiles/reconstruction.dir/reconstruction_autogen/mocs_compilation.cpp.o: CMakeFiles/reconstruction.dir/flags.make
CMakeFiles/reconstruction.dir/reconstruction_autogen/mocs_compilation.cpp.o: reconstruction_autogen/mocs_compilation.cpp
CMakeFiles/reconstruction.dir/reconstruction_autogen/mocs_compilation.cpp.o: CMakeFiles/reconstruction.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/judith/Documents/M1/POM/pom-s2/tests/test_reconstruction/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/reconstruction.dir/reconstruction_autogen/mocs_compilation.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/reconstruction.dir/reconstruction_autogen/mocs_compilation.cpp.o -MF CMakeFiles/reconstruction.dir/reconstruction_autogen/mocs_compilation.cpp.o.d -o CMakeFiles/reconstruction.dir/reconstruction_autogen/mocs_compilation.cpp.o -c /home/judith/Documents/M1/POM/pom-s2/tests/test_reconstruction/build/reconstruction_autogen/mocs_compilation.cpp

CMakeFiles/reconstruction.dir/reconstruction_autogen/mocs_compilation.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/reconstruction.dir/reconstruction_autogen/mocs_compilation.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/judith/Documents/M1/POM/pom-s2/tests/test_reconstruction/build/reconstruction_autogen/mocs_compilation.cpp > CMakeFiles/reconstruction.dir/reconstruction_autogen/mocs_compilation.cpp.i

CMakeFiles/reconstruction.dir/reconstruction_autogen/mocs_compilation.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/reconstruction.dir/reconstruction_autogen/mocs_compilation.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/judith/Documents/M1/POM/pom-s2/tests/test_reconstruction/build/reconstruction_autogen/mocs_compilation.cpp -o CMakeFiles/reconstruction.dir/reconstruction_autogen/mocs_compilation.cpp.s

CMakeFiles/reconstruction.dir/progs/reconstruction.cpp.o: CMakeFiles/reconstruction.dir/flags.make
CMakeFiles/reconstruction.dir/progs/reconstruction.cpp.o: ../progs/reconstruction.cpp
CMakeFiles/reconstruction.dir/progs/reconstruction.cpp.o: CMakeFiles/reconstruction.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/judith/Documents/M1/POM/pom-s2/tests/test_reconstruction/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/reconstruction.dir/progs/reconstruction.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/reconstruction.dir/progs/reconstruction.cpp.o -MF CMakeFiles/reconstruction.dir/progs/reconstruction.cpp.o.d -o CMakeFiles/reconstruction.dir/progs/reconstruction.cpp.o -c /home/judith/Documents/M1/POM/pom-s2/tests/test_reconstruction/progs/reconstruction.cpp

CMakeFiles/reconstruction.dir/progs/reconstruction.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/reconstruction.dir/progs/reconstruction.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/judith/Documents/M1/POM/pom-s2/tests/test_reconstruction/progs/reconstruction.cpp > CMakeFiles/reconstruction.dir/progs/reconstruction.cpp.i

CMakeFiles/reconstruction.dir/progs/reconstruction.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/reconstruction.dir/progs/reconstruction.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/judith/Documents/M1/POM/pom-s2/tests/test_reconstruction/progs/reconstruction.cpp -o CMakeFiles/reconstruction.dir/progs/reconstruction.cpp.s

# Object files for target reconstruction
reconstruction_OBJECTS = \
"CMakeFiles/reconstruction.dir/reconstruction_autogen/mocs_compilation.cpp.o" \
"CMakeFiles/reconstruction.dir/progs/reconstruction.cpp.o"

# External object files for target reconstruction
reconstruction_EXTERNAL_OBJECTS =

reconstruction: CMakeFiles/reconstruction.dir/reconstruction_autogen/mocs_compilation.cpp.o
reconstruction: CMakeFiles/reconstruction.dir/progs/reconstruction.cpp.o
reconstruction: CMakeFiles/reconstruction.dir/build.make
reconstruction: liblcc_tools_lib.so
reconstruction: libCGAL_Qt5_moc_and_resources.a
reconstruction: /usr/lib/libgmpxx.so
reconstruction: /usr/lib/libmpfr.so
reconstruction: /usr/lib/libgmp.so
reconstruction: /usr/lib/libQt5OpenGL.so.5.15.4
reconstruction: /usr/lib/libQt5Svg.so.5.15.4
reconstruction: /usr/lib/libQt5Widgets.so.5.15.4
reconstruction: /usr/lib/libQt5Gui.so.5.15.4
reconstruction: /usr/lib/libQt5Core.so.5.15.4
reconstruction: CMakeFiles/reconstruction.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/judith/Documents/M1/POM/pom-s2/tests/test_reconstruction/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable reconstruction"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/reconstruction.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/reconstruction.dir/build: reconstruction
.PHONY : CMakeFiles/reconstruction.dir/build

CMakeFiles/reconstruction.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/reconstruction.dir/cmake_clean.cmake
.PHONY : CMakeFiles/reconstruction.dir/clean

CMakeFiles/reconstruction.dir/depend:
	cd /home/judith/Documents/M1/POM/pom-s2/tests/test_reconstruction/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/judith/Documents/M1/POM/pom-s2/tests/test_reconstruction /home/judith/Documents/M1/POM/pom-s2/tests/test_reconstruction /home/judith/Documents/M1/POM/pom-s2/tests/test_reconstruction/build /home/judith/Documents/M1/POM/pom-s2/tests/test_reconstruction/build /home/judith/Documents/M1/POM/pom-s2/tests/test_reconstruction/build/CMakeFiles/reconstruction.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/reconstruction.dir/depend
