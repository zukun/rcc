# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.6

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canoncical targets will work.
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

# The program to use to edit the cache.
CMAKE_EDIT_COMMAND = /usr/bin/ccmake

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/antiw/demos/vxl-1.13.0

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin"

# Include any dependencies generated for this target.
include v3p/netlib/tests/CMakeFiles/netlib_lbfgsb_example.dir/depend.make

# Include the progress variables for this target.
include v3p/netlib/tests/CMakeFiles/netlib_lbfgsb_example.dir/progress.make

# Include the compile flags for this target's objects.
include v3p/netlib/tests/CMakeFiles/netlib_lbfgsb_example.dir/flags.make

v3p/netlib/tests/CMakeFiles/netlib_lbfgsb_example.dir/lbfgsb-example1.o: v3p/netlib/tests/CMakeFiles/netlib_lbfgsb_example.dir/flags.make
v3p/netlib/tests/CMakeFiles/netlib_lbfgsb_example.dir/lbfgsb-example1.o: /home/antiw/demos/vxl-1.13.0/v3p/netlib/tests/lbfgsb-example1.c
	$(CMAKE_COMMAND) -E cmake_progress_report "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/CMakeFiles" $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object v3p/netlib/tests/CMakeFiles/netlib_lbfgsb_example.dir/lbfgsb-example1.o"
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/v3p/netlib/tests" && /usr/bin/gcc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/netlib_lbfgsb_example.dir/lbfgsb-example1.o   -c /home/antiw/demos/vxl-1.13.0/v3p/netlib/tests/lbfgsb-example1.c

v3p/netlib/tests/CMakeFiles/netlib_lbfgsb_example.dir/lbfgsb-example1.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/netlib_lbfgsb_example.dir/lbfgsb-example1.i"
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/v3p/netlib/tests" && /usr/bin/gcc  $(C_DEFINES) $(C_FLAGS) -E /home/antiw/demos/vxl-1.13.0/v3p/netlib/tests/lbfgsb-example1.c > CMakeFiles/netlib_lbfgsb_example.dir/lbfgsb-example1.i

v3p/netlib/tests/CMakeFiles/netlib_lbfgsb_example.dir/lbfgsb-example1.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/netlib_lbfgsb_example.dir/lbfgsb-example1.s"
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/v3p/netlib/tests" && /usr/bin/gcc  $(C_DEFINES) $(C_FLAGS) -S /home/antiw/demos/vxl-1.13.0/v3p/netlib/tests/lbfgsb-example1.c -o CMakeFiles/netlib_lbfgsb_example.dir/lbfgsb-example1.s

v3p/netlib/tests/CMakeFiles/netlib_lbfgsb_example.dir/lbfgsb-example1.o.requires:
.PHONY : v3p/netlib/tests/CMakeFiles/netlib_lbfgsb_example.dir/lbfgsb-example1.o.requires

v3p/netlib/tests/CMakeFiles/netlib_lbfgsb_example.dir/lbfgsb-example1.o.provides: v3p/netlib/tests/CMakeFiles/netlib_lbfgsb_example.dir/lbfgsb-example1.o.requires
	$(MAKE) -f v3p/netlib/tests/CMakeFiles/netlib_lbfgsb_example.dir/build.make v3p/netlib/tests/CMakeFiles/netlib_lbfgsb_example.dir/lbfgsb-example1.o.provides.build
.PHONY : v3p/netlib/tests/CMakeFiles/netlib_lbfgsb_example.dir/lbfgsb-example1.o.provides

v3p/netlib/tests/CMakeFiles/netlib_lbfgsb_example.dir/lbfgsb-example1.o.provides.build: v3p/netlib/tests/CMakeFiles/netlib_lbfgsb_example.dir/lbfgsb-example1.o
.PHONY : v3p/netlib/tests/CMakeFiles/netlib_lbfgsb_example.dir/lbfgsb-example1.o.provides.build

# Object files for target netlib_lbfgsb_example
netlib_lbfgsb_example_OBJECTS = \
"CMakeFiles/netlib_lbfgsb_example.dir/lbfgsb-example1.o"

# External object files for target netlib_lbfgsb_example
netlib_lbfgsb_example_EXTERNAL_OBJECTS =

v3p/netlib/tests/netlib_lbfgsb_example: v3p/netlib/tests/CMakeFiles/netlib_lbfgsb_example.dir/lbfgsb-example1.o
v3p/netlib/tests/netlib_lbfgsb_example: lib/libv3p_netlib.a
v3p/netlib/tests/netlib_lbfgsb_example: v3p/netlib/tests/CMakeFiles/netlib_lbfgsb_example.dir/build.make
v3p/netlib/tests/netlib_lbfgsb_example: v3p/netlib/tests/CMakeFiles/netlib_lbfgsb_example.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking C executable netlib_lbfgsb_example"
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/v3p/netlib/tests" && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/netlib_lbfgsb_example.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
v3p/netlib/tests/CMakeFiles/netlib_lbfgsb_example.dir/build: v3p/netlib/tests/netlib_lbfgsb_example
.PHONY : v3p/netlib/tests/CMakeFiles/netlib_lbfgsb_example.dir/build

v3p/netlib/tests/CMakeFiles/netlib_lbfgsb_example.dir/requires: v3p/netlib/tests/CMakeFiles/netlib_lbfgsb_example.dir/lbfgsb-example1.o.requires
.PHONY : v3p/netlib/tests/CMakeFiles/netlib_lbfgsb_example.dir/requires

v3p/netlib/tests/CMakeFiles/netlib_lbfgsb_example.dir/clean:
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/v3p/netlib/tests" && $(CMAKE_COMMAND) -P CMakeFiles/netlib_lbfgsb_example.dir/cmake_clean.cmake
.PHONY : v3p/netlib/tests/CMakeFiles/netlib_lbfgsb_example.dir/clean

v3p/netlib/tests/CMakeFiles/netlib_lbfgsb_example.dir/depend:
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/antiw/demos/vxl-1.13.0 /home/antiw/demos/vxl-1.13.0/v3p/netlib/tests "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin" "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/v3p/netlib/tests" "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/v3p/netlib/tests/CMakeFiles/netlib_lbfgsb_example.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : v3p/netlib/tests/CMakeFiles/netlib_lbfgsb_example.dir/depend

