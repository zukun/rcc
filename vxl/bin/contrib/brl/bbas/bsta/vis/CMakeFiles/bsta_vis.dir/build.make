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
include contrib/brl/bbas/bsta/vis/CMakeFiles/bsta_vis.dir/depend.make

# Include the progress variables for this target.
include contrib/brl/bbas/bsta/vis/CMakeFiles/bsta_vis.dir/progress.make

# Include the compile flags for this target's objects.
include contrib/brl/bbas/bsta/vis/CMakeFiles/bsta_vis.dir/flags.make

contrib/brl/bbas/bsta/vis/CMakeFiles/bsta_vis.dir/bsta_svg_tools.o: contrib/brl/bbas/bsta/vis/CMakeFiles/bsta_vis.dir/flags.make
contrib/brl/bbas/bsta/vis/CMakeFiles/bsta_vis.dir/bsta_svg_tools.o: /home/antiw/demos/vxl-1.13.0/contrib/brl/bbas/bsta/vis/bsta_svg_tools.cxx
	$(CMAKE_COMMAND) -E cmake_progress_report "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/CMakeFiles" $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object contrib/brl/bbas/bsta/vis/CMakeFiles/bsta_vis.dir/bsta_svg_tools.o"
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/contrib/brl/bbas/bsta/vis" && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/bsta_vis.dir/bsta_svg_tools.o -c /home/antiw/demos/vxl-1.13.0/contrib/brl/bbas/bsta/vis/bsta_svg_tools.cxx

contrib/brl/bbas/bsta/vis/CMakeFiles/bsta_vis.dir/bsta_svg_tools.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/bsta_vis.dir/bsta_svg_tools.i"
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/contrib/brl/bbas/bsta/vis" && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/antiw/demos/vxl-1.13.0/contrib/brl/bbas/bsta/vis/bsta_svg_tools.cxx > CMakeFiles/bsta_vis.dir/bsta_svg_tools.i

contrib/brl/bbas/bsta/vis/CMakeFiles/bsta_vis.dir/bsta_svg_tools.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/bsta_vis.dir/bsta_svg_tools.s"
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/contrib/brl/bbas/bsta/vis" && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/antiw/demos/vxl-1.13.0/contrib/brl/bbas/bsta/vis/bsta_svg_tools.cxx -o CMakeFiles/bsta_vis.dir/bsta_svg_tools.s

contrib/brl/bbas/bsta/vis/CMakeFiles/bsta_vis.dir/bsta_svg_tools.o.requires:
.PHONY : contrib/brl/bbas/bsta/vis/CMakeFiles/bsta_vis.dir/bsta_svg_tools.o.requires

contrib/brl/bbas/bsta/vis/CMakeFiles/bsta_vis.dir/bsta_svg_tools.o.provides: contrib/brl/bbas/bsta/vis/CMakeFiles/bsta_vis.dir/bsta_svg_tools.o.requires
	$(MAKE) -f contrib/brl/bbas/bsta/vis/CMakeFiles/bsta_vis.dir/build.make contrib/brl/bbas/bsta/vis/CMakeFiles/bsta_vis.dir/bsta_svg_tools.o.provides.build
.PHONY : contrib/brl/bbas/bsta/vis/CMakeFiles/bsta_vis.dir/bsta_svg_tools.o.provides

contrib/brl/bbas/bsta/vis/CMakeFiles/bsta_vis.dir/bsta_svg_tools.o.provides.build: contrib/brl/bbas/bsta/vis/CMakeFiles/bsta_vis.dir/bsta_svg_tools.o
.PHONY : contrib/brl/bbas/bsta/vis/CMakeFiles/bsta_vis.dir/bsta_svg_tools.o.provides.build

# Object files for target bsta_vis
bsta_vis_OBJECTS = \
"CMakeFiles/bsta_vis.dir/bsta_svg_tools.o"

# External object files for target bsta_vis
bsta_vis_EXTERNAL_OBJECTS =

lib/libbsta_vis.a: contrib/brl/bbas/bsta/vis/CMakeFiles/bsta_vis.dir/bsta_svg_tools.o
lib/libbsta_vis.a: contrib/brl/bbas/bsta/vis/CMakeFiles/bsta_vis.dir/build.make
lib/libbsta_vis.a: contrib/brl/bbas/bsta/vis/CMakeFiles/bsta_vis.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX static library ../../../../../lib/libbsta_vis.a"
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/contrib/brl/bbas/bsta/vis" && $(CMAKE_COMMAND) -P CMakeFiles/bsta_vis.dir/cmake_clean_target.cmake
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/contrib/brl/bbas/bsta/vis" && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/bsta_vis.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
contrib/brl/bbas/bsta/vis/CMakeFiles/bsta_vis.dir/build: lib/libbsta_vis.a
.PHONY : contrib/brl/bbas/bsta/vis/CMakeFiles/bsta_vis.dir/build

contrib/brl/bbas/bsta/vis/CMakeFiles/bsta_vis.dir/requires: contrib/brl/bbas/bsta/vis/CMakeFiles/bsta_vis.dir/bsta_svg_tools.o.requires
.PHONY : contrib/brl/bbas/bsta/vis/CMakeFiles/bsta_vis.dir/requires

contrib/brl/bbas/bsta/vis/CMakeFiles/bsta_vis.dir/clean:
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/contrib/brl/bbas/bsta/vis" && $(CMAKE_COMMAND) -P CMakeFiles/bsta_vis.dir/cmake_clean.cmake
.PHONY : contrib/brl/bbas/bsta/vis/CMakeFiles/bsta_vis.dir/clean

contrib/brl/bbas/bsta/vis/CMakeFiles/bsta_vis.dir/depend:
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/antiw/demos/vxl-1.13.0 /home/antiw/demos/vxl-1.13.0/contrib/brl/bbas/bsta/vis "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin" "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/contrib/brl/bbas/bsta/vis" "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/contrib/brl/bbas/bsta/vis/CMakeFiles/bsta_vis.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : contrib/brl/bbas/bsta/vis/CMakeFiles/bsta_vis.dir/depend

