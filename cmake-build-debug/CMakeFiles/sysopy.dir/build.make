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
CMAKE_COMMAND = /opt/clion-2020.1.1/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /opt/clion-2020.1.1/bin/cmake/linux/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/wojtek/CLionProjects/SysOpy

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/wojtek/CLionProjects/SysOpy/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/sysopy.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/sysopy.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/sysopy.dir/flags.make

CMakeFiles/sysopy.dir/Kol1-2020/ForkAndPipe/main.c.o: CMakeFiles/sysopy.dir/flags.make
CMakeFiles/sysopy.dir/Kol1-2020/ForkAndPipe/main.c.o: ../Kol1-2020/ForkAndPipe/main.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/wojtek/CLionProjects/SysOpy/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/sysopy.dir/Kol1-2020/ForkAndPipe/main.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/sysopy.dir/Kol1-2020/ForkAndPipe/main.c.o   -c /home/wojtek/CLionProjects/SysOpy/Kol1-2020/ForkAndPipe/main.c

CMakeFiles/sysopy.dir/Kol1-2020/ForkAndPipe/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/sysopy.dir/Kol1-2020/ForkAndPipe/main.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/wojtek/CLionProjects/SysOpy/Kol1-2020/ForkAndPipe/main.c > CMakeFiles/sysopy.dir/Kol1-2020/ForkAndPipe/main.c.i

CMakeFiles/sysopy.dir/Kol1-2020/ForkAndPipe/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/sysopy.dir/Kol1-2020/ForkAndPipe/main.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/wojtek/CLionProjects/SysOpy/Kol1-2020/ForkAndPipe/main.c -o CMakeFiles/sysopy.dir/Kol1-2020/ForkAndPipe/main.c.s

CMakeFiles/sysopy.dir/Kol1-2020/SignalAllButOneWithValue/main.c.o: CMakeFiles/sysopy.dir/flags.make
CMakeFiles/sysopy.dir/Kol1-2020/SignalAllButOneWithValue/main.c.o: ../Kol1-2020/SignalAllButOneWithValue/main.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/wojtek/CLionProjects/SysOpy/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/sysopy.dir/Kol1-2020/SignalAllButOneWithValue/main.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/sysopy.dir/Kol1-2020/SignalAllButOneWithValue/main.c.o   -c /home/wojtek/CLionProjects/SysOpy/Kol1-2020/SignalAllButOneWithValue/main.c

CMakeFiles/sysopy.dir/Kol1-2020/SignalAllButOneWithValue/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/sysopy.dir/Kol1-2020/SignalAllButOneWithValue/main.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/wojtek/CLionProjects/SysOpy/Kol1-2020/SignalAllButOneWithValue/main.c > CMakeFiles/sysopy.dir/Kol1-2020/SignalAllButOneWithValue/main.c.i

CMakeFiles/sysopy.dir/Kol1-2020/SignalAllButOneWithValue/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/sysopy.dir/Kol1-2020/SignalAllButOneWithValue/main.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/wojtek/CLionProjects/SysOpy/Kol1-2020/SignalAllButOneWithValue/main.c -o CMakeFiles/sysopy.dir/Kol1-2020/SignalAllButOneWithValue/main.c.s

# Object files for target sysopy
sysopy_OBJECTS = \
"CMakeFiles/sysopy.dir/Kol1-2020/ForkAndPipe/main.c.o" \
"CMakeFiles/sysopy.dir/Kol1-2020/SignalAllButOneWithValue/main.c.o"

# External object files for target sysopy
sysopy_EXTERNAL_OBJECTS =

sysopy: CMakeFiles/sysopy.dir/Kol1-2020/ForkAndPipe/main.c.o
sysopy: CMakeFiles/sysopy.dir/Kol1-2020/SignalAllButOneWithValue/main.c.o
sysopy: CMakeFiles/sysopy.dir/build.make
sysopy: CMakeFiles/sysopy.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/wojtek/CLionProjects/SysOpy/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking C executable sysopy"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/sysopy.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/sysopy.dir/build: sysopy

.PHONY : CMakeFiles/sysopy.dir/build

CMakeFiles/sysopy.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/sysopy.dir/cmake_clean.cmake
.PHONY : CMakeFiles/sysopy.dir/clean

CMakeFiles/sysopy.dir/depend:
	cd /home/wojtek/CLionProjects/SysOpy/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/wojtek/CLionProjects/SysOpy /home/wojtek/CLionProjects/SysOpy /home/wojtek/CLionProjects/SysOpy/cmake-build-debug /home/wojtek/CLionProjects/SysOpy/cmake-build-debug /home/wojtek/CLionProjects/SysOpy/cmake-build-debug/CMakeFiles/sysopy.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/sysopy.dir/depend

