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
include core/vil/examples/CMakeFiles/vil_file_example5.dir/depend.make

# Include the progress variables for this target.
include core/vil/examples/CMakeFiles/vil_file_example5.dir/progress.make

# Include the compile flags for this target's objects.
include core/vil/examples/CMakeFiles/vil_file_example5.dir/flags.make

core/vil/examples/CMakeFiles/vil_file_example5.dir/file_example5.o: core/vil/examples/CMakeFiles/vil_file_example5.dir/flags.make
core/vil/examples/CMakeFiles/vil_file_example5.dir/file_example5.o: /home/antiw/demos/vxl-1.13.0/core/vil/examples/file_example5.cxx
	$(CMAKE_COMMAND) -E cmake_progress_report "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/CMakeFiles" $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object core/vil/examples/CMakeFiles/vil_file_example5.dir/file_example5.o"
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/core/vil/examples" && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/vil_file_example5.dir/file_example5.o -c /home/antiw/demos/vxl-1.13.0/core/vil/examples/file_example5.cxx

core/vil/examples/CMakeFiles/vil_file_example5.dir/file_example5.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/vil_file_example5.dir/file_example5.i"
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/core/vil/examples" && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/antiw/demos/vxl-1.13.0/core/vil/examples/file_example5.cxx > CMakeFiles/vil_file_example5.dir/file_example5.i

core/vil/examples/CMakeFiles/vil_file_example5.dir/file_example5.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/vil_file_example5.dir/file_example5.s"
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/core/vil/examples" && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/antiw/demos/vxl-1.13.0/core/vil/examples/file_example5.cxx -o CMakeFiles/vil_file_example5.dir/file_example5.s

core/vil/examples/CMakeFiles/vil_file_example5.dir/file_example5.o.requires:
.PHONY : core/vil/examples/CMakeFiles/vil_file_example5.dir/file_example5.o.requires

core/vil/examples/CMakeFiles/vil_file_example5.dir/file_example5.o.provides: core/vil/examples/CMakeFiles/vil_file_example5.dir/file_example5.o.requires
	$(MAKE) -f core/vil/examples/CMakeFiles/vil_file_example5.dir/build.make core/vil/examples/CMakeFiles/vil_file_example5.dir/file_example5.o.provides.build
.PHONY : core/vil/examples/CMakeFiles/vil_file_example5.dir/file_example5.o.provides

core/vil/examples/CMakeFiles/vil_file_example5.dir/file_example5.o.provides.build: core/vil/examples/CMakeFiles/vil_file_example5.dir/file_example5.o
.PHONY : core/vil/examples/CMakeFiles/vil_file_example5.dir/file_example5.o.provides.build

# Object files for target vil_file_example5
vil_file_example5_OBJECTS = \
"CMakeFiles/vil_file_example5.dir/file_example5.o"

# External object files for target vil_file_example5
vil_file_example5_EXTERNAL_OBJECTS =

core/vil/examples/vil_file_example5: core/vil/examples/CMakeFiles/vil_file_example5.dir/file_example5.o
core/vil/examples/vil_file_example5: lib/libvil.a
core/vil/examples/vil_file_example5: lib/libvcl.a
core/vil/examples/vil_file_example5: /usr/lib/libjpeg.so
core/vil/examples/vil_file_example5: lib/libgeotiff.a
core/vil/examples/vil_file_example5: /usr/lib/libtiff.so
core/vil/examples/vil_file_example5: /usr/lib/libpng.so
core/vil/examples/vil_file_example5: /usr/lib/libz.so
core/vil/examples/vil_file_example5: core/vil/examples/CMakeFiles/vil_file_example5.dir/build.make
core/vil/examples/vil_file_example5: core/vil/examples/CMakeFiles/vil_file_example5.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable vil_file_example5"
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/core/vil/examples" && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/vil_file_example5.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
core/vil/examples/CMakeFiles/vil_file_example5.dir/build: core/vil/examples/vil_file_example5
.PHONY : core/vil/examples/CMakeFiles/vil_file_example5.dir/build

core/vil/examples/CMakeFiles/vil_file_example5.dir/requires: core/vil/examples/CMakeFiles/vil_file_example5.dir/file_example5.o.requires
.PHONY : core/vil/examples/CMakeFiles/vil_file_example5.dir/requires

core/vil/examples/CMakeFiles/vil_file_example5.dir/clean:
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/core/vil/examples" && $(CMAKE_COMMAND) -P CMakeFiles/vil_file_example5.dir/cmake_clean.cmake
.PHONY : core/vil/examples/CMakeFiles/vil_file_example5.dir/clean

core/vil/examples/CMakeFiles/vil_file_example5.dir/depend:
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/antiw/demos/vxl-1.13.0 /home/antiw/demos/vxl-1.13.0/core/vil/examples "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin" "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/core/vil/examples" "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/core/vil/examples/CMakeFiles/vil_file_example5.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : core/vil/examples/CMakeFiles/vil_file_example5.dir/depend

