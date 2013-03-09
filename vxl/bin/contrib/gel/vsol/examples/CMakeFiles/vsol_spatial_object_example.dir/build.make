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
include contrib/gel/vsol/examples/CMakeFiles/vsol_spatial_object_example.dir/depend.make

# Include the progress variables for this target.
include contrib/gel/vsol/examples/CMakeFiles/vsol_spatial_object_example.dir/progress.make

# Include the compile flags for this target's objects.
include contrib/gel/vsol/examples/CMakeFiles/vsol_spatial_object_example.dir/flags.make

contrib/gel/vsol/examples/CMakeFiles/vsol_spatial_object_example.dir/vsol_spatial_object_example.o: contrib/gel/vsol/examples/CMakeFiles/vsol_spatial_object_example.dir/flags.make
contrib/gel/vsol/examples/CMakeFiles/vsol_spatial_object_example.dir/vsol_spatial_object_example.o: /home/antiw/demos/vxl-1.13.0/contrib/gel/vsol/examples/vsol_spatial_object_example.cxx
	$(CMAKE_COMMAND) -E cmake_progress_report "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/CMakeFiles" $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object contrib/gel/vsol/examples/CMakeFiles/vsol_spatial_object_example.dir/vsol_spatial_object_example.o"
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/contrib/gel/vsol/examples" && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/vsol_spatial_object_example.dir/vsol_spatial_object_example.o -c /home/antiw/demos/vxl-1.13.0/contrib/gel/vsol/examples/vsol_spatial_object_example.cxx

contrib/gel/vsol/examples/CMakeFiles/vsol_spatial_object_example.dir/vsol_spatial_object_example.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/vsol_spatial_object_example.dir/vsol_spatial_object_example.i"
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/contrib/gel/vsol/examples" && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/antiw/demos/vxl-1.13.0/contrib/gel/vsol/examples/vsol_spatial_object_example.cxx > CMakeFiles/vsol_spatial_object_example.dir/vsol_spatial_object_example.i

contrib/gel/vsol/examples/CMakeFiles/vsol_spatial_object_example.dir/vsol_spatial_object_example.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/vsol_spatial_object_example.dir/vsol_spatial_object_example.s"
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/contrib/gel/vsol/examples" && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/antiw/demos/vxl-1.13.0/contrib/gel/vsol/examples/vsol_spatial_object_example.cxx -o CMakeFiles/vsol_spatial_object_example.dir/vsol_spatial_object_example.s

contrib/gel/vsol/examples/CMakeFiles/vsol_spatial_object_example.dir/vsol_spatial_object_example.o.requires:
.PHONY : contrib/gel/vsol/examples/CMakeFiles/vsol_spatial_object_example.dir/vsol_spatial_object_example.o.requires

contrib/gel/vsol/examples/CMakeFiles/vsol_spatial_object_example.dir/vsol_spatial_object_example.o.provides: contrib/gel/vsol/examples/CMakeFiles/vsol_spatial_object_example.dir/vsol_spatial_object_example.o.requires
	$(MAKE) -f contrib/gel/vsol/examples/CMakeFiles/vsol_spatial_object_example.dir/build.make contrib/gel/vsol/examples/CMakeFiles/vsol_spatial_object_example.dir/vsol_spatial_object_example.o.provides.build
.PHONY : contrib/gel/vsol/examples/CMakeFiles/vsol_spatial_object_example.dir/vsol_spatial_object_example.o.provides

contrib/gel/vsol/examples/CMakeFiles/vsol_spatial_object_example.dir/vsol_spatial_object_example.o.provides.build: contrib/gel/vsol/examples/CMakeFiles/vsol_spatial_object_example.dir/vsol_spatial_object_example.o
.PHONY : contrib/gel/vsol/examples/CMakeFiles/vsol_spatial_object_example.dir/vsol_spatial_object_example.o.provides.build

# Object files for target vsol_spatial_object_example
vsol_spatial_object_example_OBJECTS = \
"CMakeFiles/vsol_spatial_object_example.dir/vsol_spatial_object_example.o"

# External object files for target vsol_spatial_object_example
vsol_spatial_object_example_EXTERNAL_OBJECTS =

contrib/gel/vsol/examples/vsol_spatial_object_example: contrib/gel/vsol/examples/CMakeFiles/vsol_spatial_object_example.dir/vsol_spatial_object_example.o
contrib/gel/vsol/examples/vsol_spatial_object_example: lib/libvsol.a
contrib/gel/vsol/examples/vsol_spatial_object_example: lib/libvgl_algo.a
contrib/gel/vsol/examples/vsol_spatial_object_example: lib/libvnl_algo.a
contrib/gel/vsol/examples/vsol_spatial_object_example: lib/libnetlib.a
contrib/gel/vsol/examples/vsol_spatial_object_example: lib/libv3p_netlib.a
contrib/gel/vsol/examples/vsol_spatial_object_example: lib/libvgl_io.a
contrib/gel/vsol/examples/vsol_spatial_object_example: lib/libvgl.a
contrib/gel/vsol/examples/vsol_spatial_object_example: lib/libvnl.a
contrib/gel/vsol/examples/vsol_spatial_object_example: lib/libvbl_io.a
contrib/gel/vsol/examples/vsol_spatial_object_example: lib/libvbl.a
contrib/gel/vsol/examples/vsol_spatial_object_example: lib/libvsl.a
contrib/gel/vsol/examples/vsol_spatial_object_example: lib/libvul.a
contrib/gel/vsol/examples/vsol_spatial_object_example: lib/libvcl.a
contrib/gel/vsol/examples/vsol_spatial_object_example: contrib/gel/vsol/examples/CMakeFiles/vsol_spatial_object_example.dir/build.make
contrib/gel/vsol/examples/vsol_spatial_object_example: contrib/gel/vsol/examples/CMakeFiles/vsol_spatial_object_example.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable vsol_spatial_object_example"
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/contrib/gel/vsol/examples" && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/vsol_spatial_object_example.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
contrib/gel/vsol/examples/CMakeFiles/vsol_spatial_object_example.dir/build: contrib/gel/vsol/examples/vsol_spatial_object_example
.PHONY : contrib/gel/vsol/examples/CMakeFiles/vsol_spatial_object_example.dir/build

contrib/gel/vsol/examples/CMakeFiles/vsol_spatial_object_example.dir/requires: contrib/gel/vsol/examples/CMakeFiles/vsol_spatial_object_example.dir/vsol_spatial_object_example.o.requires
.PHONY : contrib/gel/vsol/examples/CMakeFiles/vsol_spatial_object_example.dir/requires

contrib/gel/vsol/examples/CMakeFiles/vsol_spatial_object_example.dir/clean:
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/contrib/gel/vsol/examples" && $(CMAKE_COMMAND) -P CMakeFiles/vsol_spatial_object_example.dir/cmake_clean.cmake
.PHONY : contrib/gel/vsol/examples/CMakeFiles/vsol_spatial_object_example.dir/clean

contrib/gel/vsol/examples/CMakeFiles/vsol_spatial_object_example.dir/depend:
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/antiw/demos/vxl-1.13.0 /home/antiw/demos/vxl-1.13.0/contrib/gel/vsol/examples "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin" "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/contrib/gel/vsol/examples" "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/contrib/gel/vsol/examples/CMakeFiles/vsol_spatial_object_example.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : contrib/gel/vsol/examples/CMakeFiles/vsol_spatial_object_example.dir/depend

