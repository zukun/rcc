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
include contrib/brl/bbas/bmsh3d/tests/CMakeFiles/bmsh3d_test_include.dir/depend.make

# Include the progress variables for this target.
include contrib/brl/bbas/bmsh3d/tests/CMakeFiles/bmsh3d_test_include.dir/progress.make

# Include the compile flags for this target's objects.
include contrib/brl/bbas/bmsh3d/tests/CMakeFiles/bmsh3d_test_include.dir/flags.make

contrib/brl/bbas/bmsh3d/tests/CMakeFiles/bmsh3d_test_include.dir/test_include.o: contrib/brl/bbas/bmsh3d/tests/CMakeFiles/bmsh3d_test_include.dir/flags.make
contrib/brl/bbas/bmsh3d/tests/CMakeFiles/bmsh3d_test_include.dir/test_include.o: /home/antiw/demos/vxl-1.13.0/contrib/brl/bbas/bmsh3d/tests/test_include.cxx
	$(CMAKE_COMMAND) -E cmake_progress_report "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/CMakeFiles" $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object contrib/brl/bbas/bmsh3d/tests/CMakeFiles/bmsh3d_test_include.dir/test_include.o"
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/contrib/brl/bbas/bmsh3d/tests" && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/bmsh3d_test_include.dir/test_include.o -c /home/antiw/demos/vxl-1.13.0/contrib/brl/bbas/bmsh3d/tests/test_include.cxx

contrib/brl/bbas/bmsh3d/tests/CMakeFiles/bmsh3d_test_include.dir/test_include.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/bmsh3d_test_include.dir/test_include.i"
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/contrib/brl/bbas/bmsh3d/tests" && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/antiw/demos/vxl-1.13.0/contrib/brl/bbas/bmsh3d/tests/test_include.cxx > CMakeFiles/bmsh3d_test_include.dir/test_include.i

contrib/brl/bbas/bmsh3d/tests/CMakeFiles/bmsh3d_test_include.dir/test_include.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/bmsh3d_test_include.dir/test_include.s"
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/contrib/brl/bbas/bmsh3d/tests" && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/antiw/demos/vxl-1.13.0/contrib/brl/bbas/bmsh3d/tests/test_include.cxx -o CMakeFiles/bmsh3d_test_include.dir/test_include.s

contrib/brl/bbas/bmsh3d/tests/CMakeFiles/bmsh3d_test_include.dir/test_include.o.requires:
.PHONY : contrib/brl/bbas/bmsh3d/tests/CMakeFiles/bmsh3d_test_include.dir/test_include.o.requires

contrib/brl/bbas/bmsh3d/tests/CMakeFiles/bmsh3d_test_include.dir/test_include.o.provides: contrib/brl/bbas/bmsh3d/tests/CMakeFiles/bmsh3d_test_include.dir/test_include.o.requires
	$(MAKE) -f contrib/brl/bbas/bmsh3d/tests/CMakeFiles/bmsh3d_test_include.dir/build.make contrib/brl/bbas/bmsh3d/tests/CMakeFiles/bmsh3d_test_include.dir/test_include.o.provides.build
.PHONY : contrib/brl/bbas/bmsh3d/tests/CMakeFiles/bmsh3d_test_include.dir/test_include.o.provides

contrib/brl/bbas/bmsh3d/tests/CMakeFiles/bmsh3d_test_include.dir/test_include.o.provides.build: contrib/brl/bbas/bmsh3d/tests/CMakeFiles/bmsh3d_test_include.dir/test_include.o
.PHONY : contrib/brl/bbas/bmsh3d/tests/CMakeFiles/bmsh3d_test_include.dir/test_include.o.provides.build

# Object files for target bmsh3d_test_include
bmsh3d_test_include_OBJECTS = \
"CMakeFiles/bmsh3d_test_include.dir/test_include.o"

# External object files for target bmsh3d_test_include
bmsh3d_test_include_EXTERNAL_OBJECTS =

contrib/brl/bbas/bmsh3d/tests/bmsh3d_test_include: contrib/brl/bbas/bmsh3d/tests/CMakeFiles/bmsh3d_test_include.dir/test_include.o
contrib/brl/bbas/bmsh3d/tests/bmsh3d_test_include: lib/libbmsh3d.a
contrib/brl/bbas/bmsh3d/tests/bmsh3d_test_include: lib/libvgl_algo.a
contrib/brl/bbas/bmsh3d/tests/bmsh3d_test_include: lib/libvgl.a
contrib/brl/bbas/bmsh3d/tests/bmsh3d_test_include: lib/libvnl_algo.a
contrib/brl/bbas/bmsh3d/tests/bmsh3d_test_include: lib/libnetlib.a
contrib/brl/bbas/bmsh3d/tests/bmsh3d_test_include: lib/libv3p_netlib.a
contrib/brl/bbas/bmsh3d/tests/bmsh3d_test_include: lib/libvnl.a
contrib/brl/bbas/bmsh3d/tests/bmsh3d_test_include: lib/libvul.a
contrib/brl/bbas/bmsh3d/tests/bmsh3d_test_include: lib/libvcl.a
contrib/brl/bbas/bmsh3d/tests/bmsh3d_test_include: contrib/brl/bbas/bmsh3d/tests/CMakeFiles/bmsh3d_test_include.dir/build.make
contrib/brl/bbas/bmsh3d/tests/bmsh3d_test_include: contrib/brl/bbas/bmsh3d/tests/CMakeFiles/bmsh3d_test_include.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable bmsh3d_test_include"
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/contrib/brl/bbas/bmsh3d/tests" && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/bmsh3d_test_include.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
contrib/brl/bbas/bmsh3d/tests/CMakeFiles/bmsh3d_test_include.dir/build: contrib/brl/bbas/bmsh3d/tests/bmsh3d_test_include
.PHONY : contrib/brl/bbas/bmsh3d/tests/CMakeFiles/bmsh3d_test_include.dir/build

contrib/brl/bbas/bmsh3d/tests/CMakeFiles/bmsh3d_test_include.dir/requires: contrib/brl/bbas/bmsh3d/tests/CMakeFiles/bmsh3d_test_include.dir/test_include.o.requires
.PHONY : contrib/brl/bbas/bmsh3d/tests/CMakeFiles/bmsh3d_test_include.dir/requires

contrib/brl/bbas/bmsh3d/tests/CMakeFiles/bmsh3d_test_include.dir/clean:
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/contrib/brl/bbas/bmsh3d/tests" && $(CMAKE_COMMAND) -P CMakeFiles/bmsh3d_test_include.dir/cmake_clean.cmake
.PHONY : contrib/brl/bbas/bmsh3d/tests/CMakeFiles/bmsh3d_test_include.dir/clean

contrib/brl/bbas/bmsh3d/tests/CMakeFiles/bmsh3d_test_include.dir/depend:
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/antiw/demos/vxl-1.13.0 /home/antiw/demos/vxl-1.13.0/contrib/brl/bbas/bmsh3d/tests "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin" "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/contrib/brl/bbas/bmsh3d/tests" "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/contrib/brl/bbas/bmsh3d/tests/CMakeFiles/bmsh3d_test_include.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : contrib/brl/bbas/bmsh3d/tests/CMakeFiles/bmsh3d_test_include.dir/depend

