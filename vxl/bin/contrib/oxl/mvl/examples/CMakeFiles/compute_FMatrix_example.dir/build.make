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
include contrib/oxl/mvl/examples/CMakeFiles/compute_FMatrix_example.dir/depend.make

# Include the progress variables for this target.
include contrib/oxl/mvl/examples/CMakeFiles/compute_FMatrix_example.dir/progress.make

# Include the compile flags for this target's objects.
include contrib/oxl/mvl/examples/CMakeFiles/compute_FMatrix_example.dir/flags.make

contrib/oxl/mvl/examples/CMakeFiles/compute_FMatrix_example.dir/compute_FMatrix_example.o: contrib/oxl/mvl/examples/CMakeFiles/compute_FMatrix_example.dir/flags.make
contrib/oxl/mvl/examples/CMakeFiles/compute_FMatrix_example.dir/compute_FMatrix_example.o: /home/antiw/demos/vxl-1.13.0/contrib/oxl/mvl/examples/compute_FMatrix_example.cxx
	$(CMAKE_COMMAND) -E cmake_progress_report "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/CMakeFiles" $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object contrib/oxl/mvl/examples/CMakeFiles/compute_FMatrix_example.dir/compute_FMatrix_example.o"
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/contrib/oxl/mvl/examples" && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/compute_FMatrix_example.dir/compute_FMatrix_example.o -c /home/antiw/demos/vxl-1.13.0/contrib/oxl/mvl/examples/compute_FMatrix_example.cxx

contrib/oxl/mvl/examples/CMakeFiles/compute_FMatrix_example.dir/compute_FMatrix_example.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/compute_FMatrix_example.dir/compute_FMatrix_example.i"
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/contrib/oxl/mvl/examples" && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/antiw/demos/vxl-1.13.0/contrib/oxl/mvl/examples/compute_FMatrix_example.cxx > CMakeFiles/compute_FMatrix_example.dir/compute_FMatrix_example.i

contrib/oxl/mvl/examples/CMakeFiles/compute_FMatrix_example.dir/compute_FMatrix_example.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/compute_FMatrix_example.dir/compute_FMatrix_example.s"
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/contrib/oxl/mvl/examples" && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/antiw/demos/vxl-1.13.0/contrib/oxl/mvl/examples/compute_FMatrix_example.cxx -o CMakeFiles/compute_FMatrix_example.dir/compute_FMatrix_example.s

contrib/oxl/mvl/examples/CMakeFiles/compute_FMatrix_example.dir/compute_FMatrix_example.o.requires:
.PHONY : contrib/oxl/mvl/examples/CMakeFiles/compute_FMatrix_example.dir/compute_FMatrix_example.o.requires

contrib/oxl/mvl/examples/CMakeFiles/compute_FMatrix_example.dir/compute_FMatrix_example.o.provides: contrib/oxl/mvl/examples/CMakeFiles/compute_FMatrix_example.dir/compute_FMatrix_example.o.requires
	$(MAKE) -f contrib/oxl/mvl/examples/CMakeFiles/compute_FMatrix_example.dir/build.make contrib/oxl/mvl/examples/CMakeFiles/compute_FMatrix_example.dir/compute_FMatrix_example.o.provides.build
.PHONY : contrib/oxl/mvl/examples/CMakeFiles/compute_FMatrix_example.dir/compute_FMatrix_example.o.provides

contrib/oxl/mvl/examples/CMakeFiles/compute_FMatrix_example.dir/compute_FMatrix_example.o.provides.build: contrib/oxl/mvl/examples/CMakeFiles/compute_FMatrix_example.dir/compute_FMatrix_example.o
.PHONY : contrib/oxl/mvl/examples/CMakeFiles/compute_FMatrix_example.dir/compute_FMatrix_example.o.provides.build

# Object files for target compute_FMatrix_example
compute_FMatrix_example_OBJECTS = \
"CMakeFiles/compute_FMatrix_example.dir/compute_FMatrix_example.o"

# External object files for target compute_FMatrix_example
compute_FMatrix_example_EXTERNAL_OBJECTS =

contrib/oxl/mvl/examples/compute_FMatrix_example: contrib/oxl/mvl/examples/CMakeFiles/compute_FMatrix_example.dir/compute_FMatrix_example.o
contrib/oxl/mvl/examples/compute_FMatrix_example: lib/libmvl.a
contrib/oxl/mvl/examples/compute_FMatrix_example: lib/libvnl.a
contrib/oxl/mvl/examples/compute_FMatrix_example: lib/libvul.a
contrib/oxl/mvl/examples/compute_FMatrix_example: lib/libvil1.a
contrib/oxl/mvl/examples/compute_FMatrix_example: /usr/lib/libjpeg.so
contrib/oxl/mvl/examples/compute_FMatrix_example: /usr/lib/libtiff.so
contrib/oxl/mvl/examples/compute_FMatrix_example: /usr/lib/libpng.so
contrib/oxl/mvl/examples/compute_FMatrix_example: /usr/lib/libz.so
contrib/oxl/mvl/examples/compute_FMatrix_example: lib/libvbl.a
contrib/oxl/mvl/examples/compute_FMatrix_example: lib/libvgl_algo.a
contrib/oxl/mvl/examples/compute_FMatrix_example: lib/libvnl_algo.a
contrib/oxl/mvl/examples/compute_FMatrix_example: lib/libvnl.a
contrib/oxl/mvl/examples/compute_FMatrix_example: lib/libnetlib.a
contrib/oxl/mvl/examples/compute_FMatrix_example: lib/libv3p_netlib.a
contrib/oxl/mvl/examples/compute_FMatrix_example: lib/libvgl.a
contrib/oxl/mvl/examples/compute_FMatrix_example: lib/libvcl.a
contrib/oxl/mvl/examples/compute_FMatrix_example: contrib/oxl/mvl/examples/CMakeFiles/compute_FMatrix_example.dir/build.make
contrib/oxl/mvl/examples/compute_FMatrix_example: contrib/oxl/mvl/examples/CMakeFiles/compute_FMatrix_example.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable compute_FMatrix_example"
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/contrib/oxl/mvl/examples" && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/compute_FMatrix_example.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
contrib/oxl/mvl/examples/CMakeFiles/compute_FMatrix_example.dir/build: contrib/oxl/mvl/examples/compute_FMatrix_example
.PHONY : contrib/oxl/mvl/examples/CMakeFiles/compute_FMatrix_example.dir/build

contrib/oxl/mvl/examples/CMakeFiles/compute_FMatrix_example.dir/requires: contrib/oxl/mvl/examples/CMakeFiles/compute_FMatrix_example.dir/compute_FMatrix_example.o.requires
.PHONY : contrib/oxl/mvl/examples/CMakeFiles/compute_FMatrix_example.dir/requires

contrib/oxl/mvl/examples/CMakeFiles/compute_FMatrix_example.dir/clean:
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/contrib/oxl/mvl/examples" && $(CMAKE_COMMAND) -P CMakeFiles/compute_FMatrix_example.dir/cmake_clean.cmake
.PHONY : contrib/oxl/mvl/examples/CMakeFiles/compute_FMatrix_example.dir/clean

contrib/oxl/mvl/examples/CMakeFiles/compute_FMatrix_example.dir/depend:
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/antiw/demos/vxl-1.13.0 /home/antiw/demos/vxl-1.13.0/contrib/oxl/mvl/examples "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin" "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/contrib/oxl/mvl/examples" "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/contrib/oxl/mvl/examples/CMakeFiles/compute_FMatrix_example.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : contrib/oxl/mvl/examples/CMakeFiles/compute_FMatrix_example.dir/depend

