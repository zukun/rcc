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
include contrib/gel/gmvl/examples/CMakeFiles/gmvl_examples.dir/depend.make

# Include the progress variables for this target.
include contrib/gel/gmvl/examples/CMakeFiles/gmvl_examples.dir/progress.make

# Include the compile flags for this target's objects.
include contrib/gel/gmvl/examples/CMakeFiles/gmvl_examples.dir/flags.make

contrib/gel/gmvl/examples/CMakeFiles/gmvl_examples.dir/gmvl_test.o: contrib/gel/gmvl/examples/CMakeFiles/gmvl_examples.dir/flags.make
contrib/gel/gmvl/examples/CMakeFiles/gmvl_examples.dir/gmvl_test.o: /home/antiw/demos/vxl-1.13.0/contrib/gel/gmvl/examples/gmvl_test.cxx
	$(CMAKE_COMMAND) -E cmake_progress_report "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/CMakeFiles" $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object contrib/gel/gmvl/examples/CMakeFiles/gmvl_examples.dir/gmvl_test.o"
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/contrib/gel/gmvl/examples" && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/gmvl_examples.dir/gmvl_test.o -c /home/antiw/demos/vxl-1.13.0/contrib/gel/gmvl/examples/gmvl_test.cxx

contrib/gel/gmvl/examples/CMakeFiles/gmvl_examples.dir/gmvl_test.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/gmvl_examples.dir/gmvl_test.i"
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/contrib/gel/gmvl/examples" && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/antiw/demos/vxl-1.13.0/contrib/gel/gmvl/examples/gmvl_test.cxx > CMakeFiles/gmvl_examples.dir/gmvl_test.i

contrib/gel/gmvl/examples/CMakeFiles/gmvl_examples.dir/gmvl_test.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/gmvl_examples.dir/gmvl_test.s"
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/contrib/gel/gmvl/examples" && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/antiw/demos/vxl-1.13.0/contrib/gel/gmvl/examples/gmvl_test.cxx -o CMakeFiles/gmvl_examples.dir/gmvl_test.s

contrib/gel/gmvl/examples/CMakeFiles/gmvl_examples.dir/gmvl_test.o.requires:
.PHONY : contrib/gel/gmvl/examples/CMakeFiles/gmvl_examples.dir/gmvl_test.o.requires

contrib/gel/gmvl/examples/CMakeFiles/gmvl_examples.dir/gmvl_test.o.provides: contrib/gel/gmvl/examples/CMakeFiles/gmvl_examples.dir/gmvl_test.o.requires
	$(MAKE) -f contrib/gel/gmvl/examples/CMakeFiles/gmvl_examples.dir/build.make contrib/gel/gmvl/examples/CMakeFiles/gmvl_examples.dir/gmvl_test.o.provides.build
.PHONY : contrib/gel/gmvl/examples/CMakeFiles/gmvl_examples.dir/gmvl_test.o.provides

contrib/gel/gmvl/examples/CMakeFiles/gmvl_examples.dir/gmvl_test.o.provides.build: contrib/gel/gmvl/examples/CMakeFiles/gmvl_examples.dir/gmvl_test.o
.PHONY : contrib/gel/gmvl/examples/CMakeFiles/gmvl_examples.dir/gmvl_test.o.provides.build

# Object files for target gmvl_examples
gmvl_examples_OBJECTS = \
"CMakeFiles/gmvl_examples.dir/gmvl_test.o"

# External object files for target gmvl_examples
gmvl_examples_EXTERNAL_OBJECTS =

contrib/gel/gmvl/examples/gmvl_examples: contrib/gel/gmvl/examples/CMakeFiles/gmvl_examples.dir/gmvl_test.o
contrib/gel/gmvl/examples/gmvl_examples: lib/libgmvl.a
contrib/gel/gmvl/examples/gmvl_examples: lib/libvtol.a
contrib/gel/gmvl/examples/gmvl_examples: lib/libvdgl.a
contrib/gel/gmvl/examples/gmvl_examples: lib/libvsol.a
contrib/gel/gmvl/examples/gmvl_examples: lib/libvgl_algo.a
contrib/gel/gmvl/examples/gmvl_examples: lib/libvnl_algo.a
contrib/gel/gmvl/examples/gmvl_examples: lib/libnetlib.a
contrib/gel/gmvl/examples/gmvl_examples: lib/libv3p_netlib.a
contrib/gel/gmvl/examples/gmvl_examples: lib/libvbl_io.a
contrib/gel/gmvl/examples/gmvl_examples: lib/libvgl_io.a
contrib/gel/gmvl/examples/gmvl_examples: lib/libvgl.a
contrib/gel/gmvl/examples/gmvl_examples: lib/libvsl.a
contrib/gel/gmvl/examples/gmvl_examples: lib/libvnl.a
contrib/gel/gmvl/examples/gmvl_examples: lib/libvul.a
contrib/gel/gmvl/examples/gmvl_examples: lib/libvil1.a
contrib/gel/gmvl/examples/gmvl_examples: /usr/lib/libjpeg.so
contrib/gel/gmvl/examples/gmvl_examples: /usr/lib/libtiff.so
contrib/gel/gmvl/examples/gmvl_examples: /usr/lib/libpng.so
contrib/gel/gmvl/examples/gmvl_examples: /usr/lib/libz.so
contrib/gel/gmvl/examples/gmvl_examples: lib/libvbl.a
contrib/gel/gmvl/examples/gmvl_examples: lib/libvcl.a
contrib/gel/gmvl/examples/gmvl_examples: contrib/gel/gmvl/examples/CMakeFiles/gmvl_examples.dir/build.make
contrib/gel/gmvl/examples/gmvl_examples: contrib/gel/gmvl/examples/CMakeFiles/gmvl_examples.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable gmvl_examples"
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/contrib/gel/gmvl/examples" && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/gmvl_examples.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
contrib/gel/gmvl/examples/CMakeFiles/gmvl_examples.dir/build: contrib/gel/gmvl/examples/gmvl_examples
.PHONY : contrib/gel/gmvl/examples/CMakeFiles/gmvl_examples.dir/build

contrib/gel/gmvl/examples/CMakeFiles/gmvl_examples.dir/requires: contrib/gel/gmvl/examples/CMakeFiles/gmvl_examples.dir/gmvl_test.o.requires
.PHONY : contrib/gel/gmvl/examples/CMakeFiles/gmvl_examples.dir/requires

contrib/gel/gmvl/examples/CMakeFiles/gmvl_examples.dir/clean:
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/contrib/gel/gmvl/examples" && $(CMAKE_COMMAND) -P CMakeFiles/gmvl_examples.dir/cmake_clean.cmake
.PHONY : contrib/gel/gmvl/examples/CMakeFiles/gmvl_examples.dir/clean

contrib/gel/gmvl/examples/CMakeFiles/gmvl_examples.dir/depend:
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/antiw/demos/vxl-1.13.0 /home/antiw/demos/vxl-1.13.0/contrib/gel/gmvl/examples "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin" "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/contrib/gel/gmvl/examples" "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/contrib/gel/gmvl/examples/CMakeFiles/gmvl_examples.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : contrib/gel/gmvl/examples/CMakeFiles/gmvl_examples.dir/depend

