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
include contrib/brl/bbas/bnl/algo/CMakeFiles/bnl_algo.dir/depend.make

# Include the progress variables for this target.
include contrib/brl/bbas/bnl/algo/CMakeFiles/bnl_algo.dir/progress.make

# Include the compile flags for this target's objects.
include contrib/brl/bbas/bnl/algo/CMakeFiles/bnl_algo.dir/flags.make

contrib/brl/bbas/bnl/algo/CMakeFiles/bnl_algo.dir/bnl_legendre_polynomial.o: contrib/brl/bbas/bnl/algo/CMakeFiles/bnl_algo.dir/flags.make
contrib/brl/bbas/bnl/algo/CMakeFiles/bnl_algo.dir/bnl_legendre_polynomial.o: /home/antiw/demos/vxl-1.13.0/contrib/brl/bbas/bnl/algo/bnl_legendre_polynomial.cxx
	$(CMAKE_COMMAND) -E cmake_progress_report "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/CMakeFiles" $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object contrib/brl/bbas/bnl/algo/CMakeFiles/bnl_algo.dir/bnl_legendre_polynomial.o"
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/contrib/brl/bbas/bnl/algo" && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/bnl_algo.dir/bnl_legendre_polynomial.o -c /home/antiw/demos/vxl-1.13.0/contrib/brl/bbas/bnl/algo/bnl_legendre_polynomial.cxx

contrib/brl/bbas/bnl/algo/CMakeFiles/bnl_algo.dir/bnl_legendre_polynomial.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/bnl_algo.dir/bnl_legendre_polynomial.i"
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/contrib/brl/bbas/bnl/algo" && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/antiw/demos/vxl-1.13.0/contrib/brl/bbas/bnl/algo/bnl_legendre_polynomial.cxx > CMakeFiles/bnl_algo.dir/bnl_legendre_polynomial.i

contrib/brl/bbas/bnl/algo/CMakeFiles/bnl_algo.dir/bnl_legendre_polynomial.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/bnl_algo.dir/bnl_legendre_polynomial.s"
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/contrib/brl/bbas/bnl/algo" && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/antiw/demos/vxl-1.13.0/contrib/brl/bbas/bnl/algo/bnl_legendre_polynomial.cxx -o CMakeFiles/bnl_algo.dir/bnl_legendre_polynomial.s

contrib/brl/bbas/bnl/algo/CMakeFiles/bnl_algo.dir/bnl_legendre_polynomial.o.requires:
.PHONY : contrib/brl/bbas/bnl/algo/CMakeFiles/bnl_algo.dir/bnl_legendre_polynomial.o.requires

contrib/brl/bbas/bnl/algo/CMakeFiles/bnl_algo.dir/bnl_legendre_polynomial.o.provides: contrib/brl/bbas/bnl/algo/CMakeFiles/bnl_algo.dir/bnl_legendre_polynomial.o.requires
	$(MAKE) -f contrib/brl/bbas/bnl/algo/CMakeFiles/bnl_algo.dir/build.make contrib/brl/bbas/bnl/algo/CMakeFiles/bnl_algo.dir/bnl_legendre_polynomial.o.provides.build
.PHONY : contrib/brl/bbas/bnl/algo/CMakeFiles/bnl_algo.dir/bnl_legendre_polynomial.o.provides

contrib/brl/bbas/bnl/algo/CMakeFiles/bnl_algo.dir/bnl_legendre_polynomial.o.provides.build: contrib/brl/bbas/bnl/algo/CMakeFiles/bnl_algo.dir/bnl_legendre_polynomial.o
.PHONY : contrib/brl/bbas/bnl/algo/CMakeFiles/bnl_algo.dir/bnl_legendre_polynomial.o.provides.build

# Object files for target bnl_algo
bnl_algo_OBJECTS = \
"CMakeFiles/bnl_algo.dir/bnl_legendre_polynomial.o"

# External object files for target bnl_algo
bnl_algo_EXTERNAL_OBJECTS =

lib/libbnl_algo.a: contrib/brl/bbas/bnl/algo/CMakeFiles/bnl_algo.dir/bnl_legendre_polynomial.o
lib/libbnl_algo.a: contrib/brl/bbas/bnl/algo/CMakeFiles/bnl_algo.dir/build.make
lib/libbnl_algo.a: contrib/brl/bbas/bnl/algo/CMakeFiles/bnl_algo.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX static library ../../../../../lib/libbnl_algo.a"
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/contrib/brl/bbas/bnl/algo" && $(CMAKE_COMMAND) -P CMakeFiles/bnl_algo.dir/cmake_clean_target.cmake
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/contrib/brl/bbas/bnl/algo" && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/bnl_algo.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
contrib/brl/bbas/bnl/algo/CMakeFiles/bnl_algo.dir/build: lib/libbnl_algo.a
.PHONY : contrib/brl/bbas/bnl/algo/CMakeFiles/bnl_algo.dir/build

contrib/brl/bbas/bnl/algo/CMakeFiles/bnl_algo.dir/requires: contrib/brl/bbas/bnl/algo/CMakeFiles/bnl_algo.dir/bnl_legendre_polynomial.o.requires
.PHONY : contrib/brl/bbas/bnl/algo/CMakeFiles/bnl_algo.dir/requires

contrib/brl/bbas/bnl/algo/CMakeFiles/bnl_algo.dir/clean:
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/contrib/brl/bbas/bnl/algo" && $(CMAKE_COMMAND) -P CMakeFiles/bnl_algo.dir/cmake_clean.cmake
.PHONY : contrib/brl/bbas/bnl/algo/CMakeFiles/bnl_algo.dir/clean

contrib/brl/bbas/bnl/algo/CMakeFiles/bnl_algo.dir/depend:
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/antiw/demos/vxl-1.13.0 /home/antiw/demos/vxl-1.13.0/contrib/brl/bbas/bnl/algo "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin" "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/contrib/brl/bbas/bnl/algo" "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/contrib/brl/bbas/bnl/algo/CMakeFiles/bnl_algo.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : contrib/brl/bbas/bnl/algo/CMakeFiles/bnl_algo.dir/depend

