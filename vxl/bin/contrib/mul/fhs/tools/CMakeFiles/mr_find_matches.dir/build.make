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
include contrib/mul/fhs/tools/CMakeFiles/mr_find_matches.dir/depend.make

# Include the progress variables for this target.
include contrib/mul/fhs/tools/CMakeFiles/mr_find_matches.dir/progress.make

# Include the compile flags for this target's objects.
include contrib/mul/fhs/tools/CMakeFiles/mr_find_matches.dir/flags.make

contrib/mul/fhs/tools/CMakeFiles/mr_find_matches.dir/mr_find_matches.o: contrib/mul/fhs/tools/CMakeFiles/mr_find_matches.dir/flags.make
contrib/mul/fhs/tools/CMakeFiles/mr_find_matches.dir/mr_find_matches.o: /home/antiw/demos/vxl-1.13.0/contrib/mul/fhs/tools/mr_find_matches.cxx
	$(CMAKE_COMMAND) -E cmake_progress_report "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/CMakeFiles" $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object contrib/mul/fhs/tools/CMakeFiles/mr_find_matches.dir/mr_find_matches.o"
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/contrib/mul/fhs/tools" && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/mr_find_matches.dir/mr_find_matches.o -c /home/antiw/demos/vxl-1.13.0/contrib/mul/fhs/tools/mr_find_matches.cxx

contrib/mul/fhs/tools/CMakeFiles/mr_find_matches.dir/mr_find_matches.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/mr_find_matches.dir/mr_find_matches.i"
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/contrib/mul/fhs/tools" && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/antiw/demos/vxl-1.13.0/contrib/mul/fhs/tools/mr_find_matches.cxx > CMakeFiles/mr_find_matches.dir/mr_find_matches.i

contrib/mul/fhs/tools/CMakeFiles/mr_find_matches.dir/mr_find_matches.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/mr_find_matches.dir/mr_find_matches.s"
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/contrib/mul/fhs/tools" && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/antiw/demos/vxl-1.13.0/contrib/mul/fhs/tools/mr_find_matches.cxx -o CMakeFiles/mr_find_matches.dir/mr_find_matches.s

contrib/mul/fhs/tools/CMakeFiles/mr_find_matches.dir/mr_find_matches.o.requires:
.PHONY : contrib/mul/fhs/tools/CMakeFiles/mr_find_matches.dir/mr_find_matches.o.requires

contrib/mul/fhs/tools/CMakeFiles/mr_find_matches.dir/mr_find_matches.o.provides: contrib/mul/fhs/tools/CMakeFiles/mr_find_matches.dir/mr_find_matches.o.requires
	$(MAKE) -f contrib/mul/fhs/tools/CMakeFiles/mr_find_matches.dir/build.make contrib/mul/fhs/tools/CMakeFiles/mr_find_matches.dir/mr_find_matches.o.provides.build
.PHONY : contrib/mul/fhs/tools/CMakeFiles/mr_find_matches.dir/mr_find_matches.o.provides

contrib/mul/fhs/tools/CMakeFiles/mr_find_matches.dir/mr_find_matches.o.provides.build: contrib/mul/fhs/tools/CMakeFiles/mr_find_matches.dir/mr_find_matches.o
.PHONY : contrib/mul/fhs/tools/CMakeFiles/mr_find_matches.dir/mr_find_matches.o.provides.build

# Object files for target mr_find_matches
mr_find_matches_OBJECTS = \
"CMakeFiles/mr_find_matches.dir/mr_find_matches.o"

# External object files for target mr_find_matches
mr_find_matches_EXTERNAL_OBJECTS =

contrib/mul/fhs/tools/mr_find_matches: contrib/mul/fhs/tools/CMakeFiles/mr_find_matches.dir/mr_find_matches.o
contrib/mul/fhs/tools/mr_find_matches: lib/libfhs.a
contrib/mul/fhs/tools/mr_find_matches: lib/libmbl.a
contrib/mul/fhs/tools/mr_find_matches: lib/libvimt_algo.a
contrib/mul/fhs/tools/mr_find_matches: lib/libvimt.a
contrib/mul/fhs/tools/mr_find_matches: lib/libvil_algo.a
contrib/mul/fhs/tools/mr_find_matches: lib/libvil.a
contrib/mul/fhs/tools/mr_find_matches: lib/libvul.a
contrib/mul/fhs/tools/mr_find_matches: lib/libmbl.a
contrib/mul/fhs/tools/mr_find_matches: lib/libvul.a
contrib/mul/fhs/tools/mr_find_matches: lib/libvnl_io.a
contrib/mul/fhs/tools/mr_find_matches: lib/libvgl_io.a
contrib/mul/fhs/tools/mr_find_matches: lib/libvgl.a
contrib/mul/fhs/tools/mr_find_matches: lib/libvbl_io.a
contrib/mul/fhs/tools/mr_find_matches: lib/libvbl.a
contrib/mul/fhs/tools/mr_find_matches: lib/libvil_io.a
contrib/mul/fhs/tools/mr_find_matches: lib/libvsl.a
contrib/mul/fhs/tools/mr_find_matches: lib/libvil.a
contrib/mul/fhs/tools/mr_find_matches: /usr/lib/libjpeg.so
contrib/mul/fhs/tools/mr_find_matches: lib/libgeotiff.a
contrib/mul/fhs/tools/mr_find_matches: /usr/lib/libtiff.so
contrib/mul/fhs/tools/mr_find_matches: /usr/lib/libpng.so
contrib/mul/fhs/tools/mr_find_matches: /usr/lib/libz.so
contrib/mul/fhs/tools/mr_find_matches: lib/libvnl_algo.a
contrib/mul/fhs/tools/mr_find_matches: lib/libvnl.a
contrib/mul/fhs/tools/mr_find_matches: lib/libnetlib.a
contrib/mul/fhs/tools/mr_find_matches: lib/libv3p_netlib.a
contrib/mul/fhs/tools/mr_find_matches: lib/libvcl.a
contrib/mul/fhs/tools/mr_find_matches: contrib/mul/fhs/tools/CMakeFiles/mr_find_matches.dir/build.make
contrib/mul/fhs/tools/mr_find_matches: contrib/mul/fhs/tools/CMakeFiles/mr_find_matches.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable mr_find_matches"
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/contrib/mul/fhs/tools" && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/mr_find_matches.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
contrib/mul/fhs/tools/CMakeFiles/mr_find_matches.dir/build: contrib/mul/fhs/tools/mr_find_matches
.PHONY : contrib/mul/fhs/tools/CMakeFiles/mr_find_matches.dir/build

contrib/mul/fhs/tools/CMakeFiles/mr_find_matches.dir/requires: contrib/mul/fhs/tools/CMakeFiles/mr_find_matches.dir/mr_find_matches.o.requires
.PHONY : contrib/mul/fhs/tools/CMakeFiles/mr_find_matches.dir/requires

contrib/mul/fhs/tools/CMakeFiles/mr_find_matches.dir/clean:
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/contrib/mul/fhs/tools" && $(CMAKE_COMMAND) -P CMakeFiles/mr_find_matches.dir/cmake_clean.cmake
.PHONY : contrib/mul/fhs/tools/CMakeFiles/mr_find_matches.dir/clean

contrib/mul/fhs/tools/CMakeFiles/mr_find_matches.dir/depend:
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/antiw/demos/vxl-1.13.0 /home/antiw/demos/vxl-1.13.0/contrib/mul/fhs/tools "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin" "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/contrib/mul/fhs/tools" "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/contrib/mul/fhs/tools/CMakeFiles/mr_find_matches.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : contrib/mul/fhs/tools/CMakeFiles/mr_find_matches.dir/depend

