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
CMAKE_SOURCE_DIR = /home/donghyuk/project/ProtoMissileSimulator/ProtoMissileSimulator/src/LC

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/donghyuk/project/ProtoMissileSimulator/ProtoMissileSimulator/src/LC/build

# Include any dependencies generated for this target.
include server/CMakeFiles/lc_server.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include server/CMakeFiles/lc_server.dir/compiler_depend.make

# Include the progress variables for this target.
include server/CMakeFiles/lc_server.dir/progress.make

# Include the compile flags for this target's objects.
include server/CMakeFiles/lc_server.dir/flags.make

server/CMakeFiles/lc_server.dir/lc_server.cpp.o: server/CMakeFiles/lc_server.dir/flags.make
server/CMakeFiles/lc_server.dir/lc_server.cpp.o: ../server/lc_server.cpp
server/CMakeFiles/lc_server.dir/lc_server.cpp.o: server/CMakeFiles/lc_server.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/donghyuk/project/ProtoMissileSimulator/ProtoMissileSimulator/src/LC/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object server/CMakeFiles/lc_server.dir/lc_server.cpp.o"
	cd /home/donghyuk/project/ProtoMissileSimulator/ProtoMissileSimulator/src/LC/build/server && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT server/CMakeFiles/lc_server.dir/lc_server.cpp.o -MF CMakeFiles/lc_server.dir/lc_server.cpp.o.d -o CMakeFiles/lc_server.dir/lc_server.cpp.o -c /home/donghyuk/project/ProtoMissileSimulator/ProtoMissileSimulator/src/LC/server/lc_server.cpp

server/CMakeFiles/lc_server.dir/lc_server.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/lc_server.dir/lc_server.cpp.i"
	cd /home/donghyuk/project/ProtoMissileSimulator/ProtoMissileSimulator/src/LC/build/server && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/donghyuk/project/ProtoMissileSimulator/ProtoMissileSimulator/src/LC/server/lc_server.cpp > CMakeFiles/lc_server.dir/lc_server.cpp.i

server/CMakeFiles/lc_server.dir/lc_server.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/lc_server.dir/lc_server.cpp.s"
	cd /home/donghyuk/project/ProtoMissileSimulator/ProtoMissileSimulator/src/LC/build/server && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/donghyuk/project/ProtoMissileSimulator/ProtoMissileSimulator/src/LC/server/lc_server.cpp -o CMakeFiles/lc_server.dir/lc_server.cpp.s

# Object files for target lc_server
lc_server_OBJECTS = \
"CMakeFiles/lc_server.dir/lc_server.cpp.o"

# External object files for target lc_server
lc_server_EXTERNAL_OBJECTS =

server/liblc_server.a: server/CMakeFiles/lc_server.dir/lc_server.cpp.o
server/liblc_server.a: server/CMakeFiles/lc_server.dir/build.make
server/liblc_server.a: server/CMakeFiles/lc_server.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/donghyuk/project/ProtoMissileSimulator/ProtoMissileSimulator/src/LC/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library liblc_server.a"
	cd /home/donghyuk/project/ProtoMissileSimulator/ProtoMissileSimulator/src/LC/build/server && $(CMAKE_COMMAND) -P CMakeFiles/lc_server.dir/cmake_clean_target.cmake
	cd /home/donghyuk/project/ProtoMissileSimulator/ProtoMissileSimulator/src/LC/build/server && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/lc_server.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
server/CMakeFiles/lc_server.dir/build: server/liblc_server.a
.PHONY : server/CMakeFiles/lc_server.dir/build

server/CMakeFiles/lc_server.dir/clean:
	cd /home/donghyuk/project/ProtoMissileSimulator/ProtoMissileSimulator/src/LC/build/server && $(CMAKE_COMMAND) -P CMakeFiles/lc_server.dir/cmake_clean.cmake
.PHONY : server/CMakeFiles/lc_server.dir/clean

server/CMakeFiles/lc_server.dir/depend:
	cd /home/donghyuk/project/ProtoMissileSimulator/ProtoMissileSimulator/src/LC/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/donghyuk/project/ProtoMissileSimulator/ProtoMissileSimulator/src/LC /home/donghyuk/project/ProtoMissileSimulator/ProtoMissileSimulator/src/LC/server /home/donghyuk/project/ProtoMissileSimulator/ProtoMissileSimulator/src/LC/build /home/donghyuk/project/ProtoMissileSimulator/ProtoMissileSimulator/src/LC/build/server /home/donghyuk/project/ProtoMissileSimulator/ProtoMissileSimulator/src/LC/build/server/CMakeFiles/lc_server.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : server/CMakeFiles/lc_server.dir/depend

