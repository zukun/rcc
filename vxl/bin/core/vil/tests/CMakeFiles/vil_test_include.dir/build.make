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
include core/vil/tests/CMakeFiles/vil_test_include.dir/depend.make

# Include the progress variables for this target.
include core/vil/tests/CMakeFiles/vil_test_include.dir/progress.make

# Include the compile flags for this target's objects.
include core/vil/tests/CMakeFiles/vil_test_include.dir/flags.make

core/vil/tests/CMakeFiles/vil_test_include.dir/test_include.o: core/vil/tests/CMakeFiles/vil_test_include.dir/flags.make
core/vil/tests/CMakeFiles/vil_test_include.dir/test_include.o: /home/antiw/demos/vxl-1.13.0/core/vil/tests/test_include.cxx
	$(CMAKE_COMMAND) -E cmake_progress_report "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/CMakeFiles" $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object core/vil/tests/CMakeFiles/vil_test_include.dir/test_include.o"
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/core/vil/tests" && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/vil_test_include.dir/test_include.o -c /home/antiw/demos/vxl-1.13.0/core/vil/tests/test_include.cxx

core/vil/tests/CMakeFiles/vil_test_include.dir/test_include.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/vil_test_include.dir/test_include.i"
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/core/vil/tests" && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/antiw/demos/vxl-1.13.0/core/vil/tests/test_include.cxx > CMakeFiles/vil_test_include.dir/test_include.i

core/vil/tests/CMakeFiles/vil_test_include.dir/test_include.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/vil_test_include.dir/test_include.s"
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/core/vil/tests" && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/antiw/demos/vxl-1.13.0/core/vil/tests/test_include.cxx -o CMakeFiles/vil_test_include.dir/test_include.s

core/vil/tests/CMakeFiles/vil_test_include.dir/test_include.o.requires:
.PHONY : core/vil/tests/CMakeFiles/vil_test_include.dir/test_include.o.requires

core/vil/tests/CMakeFiles/vil_test_include.dir/test_include.o.provides: core/vil/tests/CMakeFiles/vil_test_include.dir/test_include.o.requires
	$(MAKE) -f core/vil/tests/CMakeFiles/vil_test_include.dir/build.make core/vil/tests/CMakeFiles/vil_test_include.dir/test_include.o.provides.build
.PHONY : core/vil/tests/CMakeFiles/vil_test_include.dir/test_include.o.provides

core/vil/tests/CMakeFiles/vil_test_include.dir/test_include.o.provides.build: core/vil/tests/CMakeFiles/vil_test_include.dir/test_include.o
.PHONY : core/vil/tests/CMakeFiles/vil_test_include.dir/test_include.o.provides.build

# Object files for target vil_test_include
vil_test_include_OBJECTS = \
"CMakeFiles/vil_test_include.dir/test_include.o"

# External object files for target vil_test_include
vil_test_include_EXTERNAL_OBJECTS =

core/vil/tests/vil_test_include: core/vil/tests/CMakeFiles/vil_test_include.dir/test_include.o
core/vil/tests/vil_test_include: lib/libvil.a
core/vil/tests/vil_test_include: /usr/lib/libjpeg.so
core/vil/tests/vil_test_include: lib/libgeotiff.a
core/vil/tests/vil_test_include: /usr/lib/libtiff.so
core/vil/tests/vil_test_include: /usr/lib/libpng.so
core/vil/tests/vil_test_include: /usr/lib/libz.so
core/vil/tests/vil_test_include: lib/libvcl.a
core/vil/tests/vil_test_include: core/vil/tests/CMakeFiles/vil_test_include.dir/build.make
core/vil/tests/vil_test_include: core/vil/tests/CMakeFiles/vil_test_include.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable vil_test_include"
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/core/vil/tests" && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/vil_test_include.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
core/vil/tests/CMakeFiles/vil_test_include.dir/build: core/vil/tests/vil_test_include
.PHONY : core/vil/tests/CMakeFiles/vil_test_include.dir/build

core/vil/tests/CMakeFiles/vil_test_include.dir/requires: core/vil/tests/CMakeFiles/vil_test_include.dir/test_include.o.requires
.PHONY : core/vil/tests/CMakeFiles/vil_test_include.dir/requires

core/vil/tests/CMakeFiles/vil_test_include.dir/clean:
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/core/vil/tests" && $(CMAKE_COMMAND) -P CMakeFiles/vil_test_include.dir/cmake_clean.cmake
.PHONY : core/vil/tests/CMakeFiles/vil_test_include.dir/clean

core/vil/tests/CMakeFiles/vil_test_include.dir/depend:
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/antiw/demos/vxl-1.13.0 /home/antiw/demos/vxl-1.13.0/core/vil/tests "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin" "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/core/vil/tests" "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/core/vil/tests/CMakeFiles/vil_test_include.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : core/vil/tests/CMakeFiles/vil_test_include.dir/depend

