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
include core/examples/CMakeFiles/example_create_image_vil.dir/depend.make

# Include the progress variables for this target.
include core/examples/CMakeFiles/example_create_image_vil.dir/progress.make

# Include the compile flags for this target's objects.
include core/examples/CMakeFiles/example_create_image_vil.dir/flags.make

core/examples/CMakeFiles/example_create_image_vil.dir/create_image_vil.o: core/examples/CMakeFiles/example_create_image_vil.dir/flags.make
core/examples/CMakeFiles/example_create_image_vil.dir/create_image_vil.o: /home/antiw/demos/vxl-1.13.0/core/examples/create_image_vil.cxx
	$(CMAKE_COMMAND) -E cmake_progress_report "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/CMakeFiles" $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object core/examples/CMakeFiles/example_create_image_vil.dir/create_image_vil.o"
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/core/examples" && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/example_create_image_vil.dir/create_image_vil.o -c /home/antiw/demos/vxl-1.13.0/core/examples/create_image_vil.cxx

core/examples/CMakeFiles/example_create_image_vil.dir/create_image_vil.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/example_create_image_vil.dir/create_image_vil.i"
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/core/examples" && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/antiw/demos/vxl-1.13.0/core/examples/create_image_vil.cxx > CMakeFiles/example_create_image_vil.dir/create_image_vil.i

core/examples/CMakeFiles/example_create_image_vil.dir/create_image_vil.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/example_create_image_vil.dir/create_image_vil.s"
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/core/examples" && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/antiw/demos/vxl-1.13.0/core/examples/create_image_vil.cxx -o CMakeFiles/example_create_image_vil.dir/create_image_vil.s

core/examples/CMakeFiles/example_create_image_vil.dir/create_image_vil.o.requires:
.PHONY : core/examples/CMakeFiles/example_create_image_vil.dir/create_image_vil.o.requires

core/examples/CMakeFiles/example_create_image_vil.dir/create_image_vil.o.provides: core/examples/CMakeFiles/example_create_image_vil.dir/create_image_vil.o.requires
	$(MAKE) -f core/examples/CMakeFiles/example_create_image_vil.dir/build.make core/examples/CMakeFiles/example_create_image_vil.dir/create_image_vil.o.provides.build
.PHONY : core/examples/CMakeFiles/example_create_image_vil.dir/create_image_vil.o.provides

core/examples/CMakeFiles/example_create_image_vil.dir/create_image_vil.o.provides.build: core/examples/CMakeFiles/example_create_image_vil.dir/create_image_vil.o
.PHONY : core/examples/CMakeFiles/example_create_image_vil.dir/create_image_vil.o.provides.build

# Object files for target example_create_image_vil
example_create_image_vil_OBJECTS = \
"CMakeFiles/example_create_image_vil.dir/create_image_vil.o"

# External object files for target example_create_image_vil
example_create_image_vil_EXTERNAL_OBJECTS =

core/examples/example_create_image_vil: core/examples/CMakeFiles/example_create_image_vil.dir/create_image_vil.o
core/examples/example_create_image_vil: lib/libvul.a
core/examples/example_create_image_vil: lib/libvil.a
core/examples/example_create_image_vil: lib/libvcl.a
core/examples/example_create_image_vil: /usr/lib/libjpeg.so
core/examples/example_create_image_vil: lib/libgeotiff.a
core/examples/example_create_image_vil: /usr/lib/libtiff.so
core/examples/example_create_image_vil: /usr/lib/libpng.so
core/examples/example_create_image_vil: /usr/lib/libz.so
core/examples/example_create_image_vil: core/examples/CMakeFiles/example_create_image_vil.dir/build.make
core/examples/example_create_image_vil: core/examples/CMakeFiles/example_create_image_vil.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable example_create_image_vil"
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/core/examples" && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/example_create_image_vil.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
core/examples/CMakeFiles/example_create_image_vil.dir/build: core/examples/example_create_image_vil
.PHONY : core/examples/CMakeFiles/example_create_image_vil.dir/build

core/examples/CMakeFiles/example_create_image_vil.dir/requires: core/examples/CMakeFiles/example_create_image_vil.dir/create_image_vil.o.requires
.PHONY : core/examples/CMakeFiles/example_create_image_vil.dir/requires

core/examples/CMakeFiles/example_create_image_vil.dir/clean:
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/core/examples" && $(CMAKE_COMMAND) -P CMakeFiles/example_create_image_vil.dir/cmake_clean.cmake
.PHONY : core/examples/CMakeFiles/example_create_image_vil.dir/clean

core/examples/CMakeFiles/example_create_image_vil.dir/depend:
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/antiw/demos/vxl-1.13.0 /home/antiw/demos/vxl-1.13.0/core/examples "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin" "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/core/examples" "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/core/examples/CMakeFiles/example_create_image_vil.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : core/examples/CMakeFiles/example_create_image_vil.dir/depend

