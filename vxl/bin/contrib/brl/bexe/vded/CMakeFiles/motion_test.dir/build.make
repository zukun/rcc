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
include contrib/brl/bexe/vded/CMakeFiles/motion_test.dir/depend.make

# Include the progress variables for this target.
include contrib/brl/bexe/vded/CMakeFiles/motion_test.dir/progress.make

# Include the compile flags for this target's objects.
include contrib/brl/bexe/vded/CMakeFiles/motion_test.dir/flags.make

contrib/brl/bexe/vded/CMakeFiles/motion_test.dir/motion_test.o: contrib/brl/bexe/vded/CMakeFiles/motion_test.dir/flags.make
contrib/brl/bexe/vded/CMakeFiles/motion_test.dir/motion_test.o: /home/antiw/demos/vxl-1.13.0/contrib/brl/bexe/vded/motion_test.cxx
	$(CMAKE_COMMAND) -E cmake_progress_report "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/CMakeFiles" $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object contrib/brl/bexe/vded/CMakeFiles/motion_test.dir/motion_test.o"
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/contrib/brl/bexe/vded" && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/motion_test.dir/motion_test.o -c /home/antiw/demos/vxl-1.13.0/contrib/brl/bexe/vded/motion_test.cxx

contrib/brl/bexe/vded/CMakeFiles/motion_test.dir/motion_test.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/motion_test.dir/motion_test.i"
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/contrib/brl/bexe/vded" && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/antiw/demos/vxl-1.13.0/contrib/brl/bexe/vded/motion_test.cxx > CMakeFiles/motion_test.dir/motion_test.i

contrib/brl/bexe/vded/CMakeFiles/motion_test.dir/motion_test.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/motion_test.dir/motion_test.s"
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/contrib/brl/bexe/vded" && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/antiw/demos/vxl-1.13.0/contrib/brl/bexe/vded/motion_test.cxx -o CMakeFiles/motion_test.dir/motion_test.s

contrib/brl/bexe/vded/CMakeFiles/motion_test.dir/motion_test.o.requires:
.PHONY : contrib/brl/bexe/vded/CMakeFiles/motion_test.dir/motion_test.o.requires

contrib/brl/bexe/vded/CMakeFiles/motion_test.dir/motion_test.o.provides: contrib/brl/bexe/vded/CMakeFiles/motion_test.dir/motion_test.o.requires
	$(MAKE) -f contrib/brl/bexe/vded/CMakeFiles/motion_test.dir/build.make contrib/brl/bexe/vded/CMakeFiles/motion_test.dir/motion_test.o.provides.build
.PHONY : contrib/brl/bexe/vded/CMakeFiles/motion_test.dir/motion_test.o.provides

contrib/brl/bexe/vded/CMakeFiles/motion_test.dir/motion_test.o.provides.build: contrib/brl/bexe/vded/CMakeFiles/motion_test.dir/motion_test.o
.PHONY : contrib/brl/bexe/vded/CMakeFiles/motion_test.dir/motion_test.o.provides.build

# Object files for target motion_test
motion_test_OBJECTS = \
"CMakeFiles/motion_test.dir/motion_test.o"

# External object files for target motion_test
motion_test_EXTERNAL_OBJECTS =

contrib/brl/bexe/vded/motion_test: contrib/brl/bexe/vded/CMakeFiles/motion_test.dir/motion_test.o
contrib/brl/bexe/vded/motion_test: lib/libbrip.a
contrib/brl/bexe/vded/motion_test: lib/libvidl1.a
contrib/brl/bexe/vded/motion_test: lib/libvil_algo.a
contrib/brl/bexe/vded/motion_test: lib/libvil.a
contrib/brl/bexe/vded/motion_test: lib/libvul.a
contrib/brl/bexe/vded/motion_test: lib/libgevd.a
contrib/brl/bexe/vded/motion_test: lib/libvtol.a
contrib/brl/bexe/vded/motion_test: lib/libvdgl.a
contrib/brl/bexe/vded/motion_test: lib/libbsta.a
contrib/brl/bexe/vded/motion_test: lib/libbsol.a
contrib/brl/bexe/vded/motion_test: lib/libvsol.a
contrib/brl/bexe/vded/motion_test: lib/libvgl_io.a
contrib/brl/bexe/vded/motion_test: lib/libvbl_io.a
contrib/brl/bexe/vded/motion_test: lib/libvsl.a
contrib/brl/bexe/vded/motion_test: lib/libvil1.a
contrib/brl/bexe/vded/motion_test: lib/libvgl_algo.a
contrib/brl/bexe/vded/motion_test: lib/libvgl.a
contrib/brl/bexe/vded/motion_test: lib/libvnl_algo.a
contrib/brl/bexe/vded/motion_test: lib/libnetlib.a
contrib/brl/bexe/vded/motion_test: lib/libv3p_netlib.a
contrib/brl/bexe/vded/motion_test: lib/libvnl.a
contrib/brl/bexe/vded/motion_test: lib/libvil.a
contrib/brl/bexe/vded/motion_test: /usr/lib/libjpeg.so
contrib/brl/bexe/vded/motion_test: lib/libgeotiff.a
contrib/brl/bexe/vded/motion_test: /usr/lib/libtiff.so
contrib/brl/bexe/vded/motion_test: /usr/lib/libpng.so
contrib/brl/bexe/vded/motion_test: /usr/lib/libz.so
contrib/brl/bexe/vded/motion_test: lib/libvul.a
contrib/brl/bexe/vded/motion_test: lib/libvbl.a
contrib/brl/bexe/vded/motion_test: lib/libvcl.a
contrib/brl/bexe/vded/motion_test: lib/libmpeg2.a
contrib/brl/bexe/vded/motion_test: lib/libvo.a
contrib/brl/bexe/vded/motion_test: contrib/brl/bexe/vded/CMakeFiles/motion_test.dir/build.make
contrib/brl/bexe/vded/motion_test: contrib/brl/bexe/vded/CMakeFiles/motion_test.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable motion_test"
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/contrib/brl/bexe/vded" && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/motion_test.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
contrib/brl/bexe/vded/CMakeFiles/motion_test.dir/build: contrib/brl/bexe/vded/motion_test
.PHONY : contrib/brl/bexe/vded/CMakeFiles/motion_test.dir/build

contrib/brl/bexe/vded/CMakeFiles/motion_test.dir/requires: contrib/brl/bexe/vded/CMakeFiles/motion_test.dir/motion_test.o.requires
.PHONY : contrib/brl/bexe/vded/CMakeFiles/motion_test.dir/requires

contrib/brl/bexe/vded/CMakeFiles/motion_test.dir/clean:
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/contrib/brl/bexe/vded" && $(CMAKE_COMMAND) -P CMakeFiles/motion_test.dir/cmake_clean.cmake
.PHONY : contrib/brl/bexe/vded/CMakeFiles/motion_test.dir/clean

contrib/brl/bexe/vded/CMakeFiles/motion_test.dir/depend:
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/antiw/demos/vxl-1.13.0 /home/antiw/demos/vxl-1.13.0/contrib/brl/bexe/vded "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin" "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/contrib/brl/bexe/vded" "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/contrib/brl/bexe/vded/CMakeFiles/motion_test.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : contrib/brl/bexe/vded/CMakeFiles/motion_test.dir/depend

