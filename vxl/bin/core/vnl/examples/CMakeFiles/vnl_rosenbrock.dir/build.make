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
include core/vnl/examples/CMakeFiles/vnl_rosenbrock.dir/depend.make

# Include the progress variables for this target.
include core/vnl/examples/CMakeFiles/vnl_rosenbrock.dir/progress.make

# Include the compile flags for this target's objects.
include core/vnl/examples/CMakeFiles/vnl_rosenbrock.dir/flags.make

core/vnl/examples/CMakeFiles/vnl_rosenbrock.dir/vnl_rosenbrock.o: core/vnl/examples/CMakeFiles/vnl_rosenbrock.dir/flags.make
core/vnl/examples/CMakeFiles/vnl_rosenbrock.dir/vnl_rosenbrock.o: /home/antiw/demos/vxl-1.13.0/core/vnl/examples/vnl_rosenbrock.cxx
	$(CMAKE_COMMAND) -E cmake_progress_report "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/CMakeFiles" $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object core/vnl/examples/CMakeFiles/vnl_rosenbrock.dir/vnl_rosenbrock.o"
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/core/vnl/examples" && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/vnl_rosenbrock.dir/vnl_rosenbrock.o -c /home/antiw/demos/vxl-1.13.0/core/vnl/examples/vnl_rosenbrock.cxx

core/vnl/examples/CMakeFiles/vnl_rosenbrock.dir/vnl_rosenbrock.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/vnl_rosenbrock.dir/vnl_rosenbrock.i"
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/core/vnl/examples" && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/antiw/demos/vxl-1.13.0/core/vnl/examples/vnl_rosenbrock.cxx > CMakeFiles/vnl_rosenbrock.dir/vnl_rosenbrock.i

core/vnl/examples/CMakeFiles/vnl_rosenbrock.dir/vnl_rosenbrock.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/vnl_rosenbrock.dir/vnl_rosenbrock.s"
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/core/vnl/examples" && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/antiw/demos/vxl-1.13.0/core/vnl/examples/vnl_rosenbrock.cxx -o CMakeFiles/vnl_rosenbrock.dir/vnl_rosenbrock.s

core/vnl/examples/CMakeFiles/vnl_rosenbrock.dir/vnl_rosenbrock.o.requires:
.PHONY : core/vnl/examples/CMakeFiles/vnl_rosenbrock.dir/vnl_rosenbrock.o.requires

core/vnl/examples/CMakeFiles/vnl_rosenbrock.dir/vnl_rosenbrock.o.provides: core/vnl/examples/CMakeFiles/vnl_rosenbrock.dir/vnl_rosenbrock.o.requires
	$(MAKE) -f core/vnl/examples/CMakeFiles/vnl_rosenbrock.dir/build.make core/vnl/examples/CMakeFiles/vnl_rosenbrock.dir/vnl_rosenbrock.o.provides.build
.PHONY : core/vnl/examples/CMakeFiles/vnl_rosenbrock.dir/vnl_rosenbrock.o.provides

core/vnl/examples/CMakeFiles/vnl_rosenbrock.dir/vnl_rosenbrock.o.provides.build: core/vnl/examples/CMakeFiles/vnl_rosenbrock.dir/vnl_rosenbrock.o
.PHONY : core/vnl/examples/CMakeFiles/vnl_rosenbrock.dir/vnl_rosenbrock.o.provides.build

# Object files for target vnl_rosenbrock
vnl_rosenbrock_OBJECTS = \
"CMakeFiles/vnl_rosenbrock.dir/vnl_rosenbrock.o"

# External object files for target vnl_rosenbrock
vnl_rosenbrock_EXTERNAL_OBJECTS =

core/vnl/examples/vnl_rosenbrock: core/vnl/examples/CMakeFiles/vnl_rosenbrock.dir/vnl_rosenbrock.o
core/vnl/examples/vnl_rosenbrock: lib/libvnl_algo.a
core/vnl/examples/vnl_rosenbrock: lib/libvnl.a
core/vnl/examples/vnl_rosenbrock: lib/libvul.a
core/vnl/examples/vnl_rosenbrock: lib/libnetlib.a
core/vnl/examples/vnl_rosenbrock: lib/libv3p_netlib.a
core/vnl/examples/vnl_rosenbrock: lib/libvcl.a
core/vnl/examples/vnl_rosenbrock: core/vnl/examples/CMakeFiles/vnl_rosenbrock.dir/build.make
core/vnl/examples/vnl_rosenbrock: core/vnl/examples/CMakeFiles/vnl_rosenbrock.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable vnl_rosenbrock"
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/core/vnl/examples" && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/vnl_rosenbrock.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
core/vnl/examples/CMakeFiles/vnl_rosenbrock.dir/build: core/vnl/examples/vnl_rosenbrock
.PHONY : core/vnl/examples/CMakeFiles/vnl_rosenbrock.dir/build

core/vnl/examples/CMakeFiles/vnl_rosenbrock.dir/requires: core/vnl/examples/CMakeFiles/vnl_rosenbrock.dir/vnl_rosenbrock.o.requires
.PHONY : core/vnl/examples/CMakeFiles/vnl_rosenbrock.dir/requires

core/vnl/examples/CMakeFiles/vnl_rosenbrock.dir/clean:
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/core/vnl/examples" && $(CMAKE_COMMAND) -P CMakeFiles/vnl_rosenbrock.dir/cmake_clean.cmake
.PHONY : core/vnl/examples/CMakeFiles/vnl_rosenbrock.dir/clean

core/vnl/examples/CMakeFiles/vnl_rosenbrock.dir/depend:
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/antiw/demos/vxl-1.13.0 /home/antiw/demos/vxl-1.13.0/core/vnl/examples "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin" "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/core/vnl/examples" "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/core/vnl/examples/CMakeFiles/vnl_rosenbrock.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : core/vnl/examples/CMakeFiles/vnl_rosenbrock.dir/depend

