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
include core/vil1/examples/CMakeFiles/vil1_flipud.dir/depend.make

# Include the progress variables for this target.
include core/vil1/examples/CMakeFiles/vil1_flipud.dir/progress.make

# Include the compile flags for this target's objects.
include core/vil1/examples/CMakeFiles/vil1_flipud.dir/flags.make

core/vil1/examples/CMakeFiles/vil1_flipud.dir/vil1_flipud.o: core/vil1/examples/CMakeFiles/vil1_flipud.dir/flags.make
core/vil1/examples/CMakeFiles/vil1_flipud.dir/vil1_flipud.o: /home/antiw/demos/vxl-1.13.0/core/vil1/examples/vil1_flipud.cxx
	$(CMAKE_COMMAND) -E cmake_progress_report "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/CMakeFiles" $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object core/vil1/examples/CMakeFiles/vil1_flipud.dir/vil1_flipud.o"
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/core/vil1/examples" && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/vil1_flipud.dir/vil1_flipud.o -c /home/antiw/demos/vxl-1.13.0/core/vil1/examples/vil1_flipud.cxx

core/vil1/examples/CMakeFiles/vil1_flipud.dir/vil1_flipud.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/vil1_flipud.dir/vil1_flipud.i"
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/core/vil1/examples" && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/antiw/demos/vxl-1.13.0/core/vil1/examples/vil1_flipud.cxx > CMakeFiles/vil1_flipud.dir/vil1_flipud.i

core/vil1/examples/CMakeFiles/vil1_flipud.dir/vil1_flipud.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/vil1_flipud.dir/vil1_flipud.s"
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/core/vil1/examples" && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/antiw/demos/vxl-1.13.0/core/vil1/examples/vil1_flipud.cxx -o CMakeFiles/vil1_flipud.dir/vil1_flipud.s

core/vil1/examples/CMakeFiles/vil1_flipud.dir/vil1_flipud.o.requires:
.PHONY : core/vil1/examples/CMakeFiles/vil1_flipud.dir/vil1_flipud.o.requires

core/vil1/examples/CMakeFiles/vil1_flipud.dir/vil1_flipud.o.provides: core/vil1/examples/CMakeFiles/vil1_flipud.dir/vil1_flipud.o.requires
	$(MAKE) -f core/vil1/examples/CMakeFiles/vil1_flipud.dir/build.make core/vil1/examples/CMakeFiles/vil1_flipud.dir/vil1_flipud.o.provides.build
.PHONY : core/vil1/examples/CMakeFiles/vil1_flipud.dir/vil1_flipud.o.provides

core/vil1/examples/CMakeFiles/vil1_flipud.dir/vil1_flipud.o.provides.build: core/vil1/examples/CMakeFiles/vil1_flipud.dir/vil1_flipud.o
.PHONY : core/vil1/examples/CMakeFiles/vil1_flipud.dir/vil1_flipud.o.provides.build

# Object files for target vil1_flipud
vil1_flipud_OBJECTS = \
"CMakeFiles/vil1_flipud.dir/vil1_flipud.o"

# External object files for target vil1_flipud
vil1_flipud_EXTERNAL_OBJECTS =

core/vil1/examples/vil1_flipud: core/vil1/examples/CMakeFiles/vil1_flipud.dir/vil1_flipud.o
core/vil1/examples/vil1_flipud: lib/libvil1.a
core/vil1/examples/vil1_flipud: lib/libvul.a
core/vil1/examples/vil1_flipud: /usr/lib/libjpeg.so
core/vil1/examples/vil1_flipud: /usr/lib/libtiff.so
core/vil1/examples/vil1_flipud: /usr/lib/libpng.so
core/vil1/examples/vil1_flipud: /usr/lib/libz.so
core/vil1/examples/vil1_flipud: lib/libvcl.a
core/vil1/examples/vil1_flipud: core/vil1/examples/CMakeFiles/vil1_flipud.dir/build.make
core/vil1/examples/vil1_flipud: core/vil1/examples/CMakeFiles/vil1_flipud.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable vil1_flipud"
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/core/vil1/examples" && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/vil1_flipud.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
core/vil1/examples/CMakeFiles/vil1_flipud.dir/build: core/vil1/examples/vil1_flipud
.PHONY : core/vil1/examples/CMakeFiles/vil1_flipud.dir/build

core/vil1/examples/CMakeFiles/vil1_flipud.dir/requires: core/vil1/examples/CMakeFiles/vil1_flipud.dir/vil1_flipud.o.requires
.PHONY : core/vil1/examples/CMakeFiles/vil1_flipud.dir/requires

core/vil1/examples/CMakeFiles/vil1_flipud.dir/clean:
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/core/vil1/examples" && $(CMAKE_COMMAND) -P CMakeFiles/vil1_flipud.dir/cmake_clean.cmake
.PHONY : core/vil1/examples/CMakeFiles/vil1_flipud.dir/clean

core/vil1/examples/CMakeFiles/vil1_flipud.dir/depend:
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/antiw/demos/vxl-1.13.0 /home/antiw/demos/vxl-1.13.0/core/vil1/examples "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin" "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/core/vil1/examples" "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/core/vil1/examples/CMakeFiles/vil1_flipud.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : core/vil1/examples/CMakeFiles/vil1_flipud.dir/depend

