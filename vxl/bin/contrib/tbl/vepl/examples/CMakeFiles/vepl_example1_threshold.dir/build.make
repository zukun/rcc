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
include contrib/tbl/vepl/examples/CMakeFiles/vepl_example1_threshold.dir/depend.make

# Include the progress variables for this target.
include contrib/tbl/vepl/examples/CMakeFiles/vepl_example1_threshold.dir/progress.make

# Include the compile flags for this target's objects.
include contrib/tbl/vepl/examples/CMakeFiles/vepl_example1_threshold.dir/flags.make

contrib/tbl/vepl/examples/CMakeFiles/vepl_example1_threshold.dir/example1_threshold.o: contrib/tbl/vepl/examples/CMakeFiles/vepl_example1_threshold.dir/flags.make
contrib/tbl/vepl/examples/CMakeFiles/vepl_example1_threshold.dir/example1_threshold.o: /home/antiw/demos/vxl-1.13.0/contrib/tbl/vepl/examples/example1_threshold.cxx
	$(CMAKE_COMMAND) -E cmake_progress_report "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/CMakeFiles" $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object contrib/tbl/vepl/examples/CMakeFiles/vepl_example1_threshold.dir/example1_threshold.o"
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/contrib/tbl/vepl/examples" && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/vepl_example1_threshold.dir/example1_threshold.o -c /home/antiw/demos/vxl-1.13.0/contrib/tbl/vepl/examples/example1_threshold.cxx

contrib/tbl/vepl/examples/CMakeFiles/vepl_example1_threshold.dir/example1_threshold.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/vepl_example1_threshold.dir/example1_threshold.i"
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/contrib/tbl/vepl/examples" && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/antiw/demos/vxl-1.13.0/contrib/tbl/vepl/examples/example1_threshold.cxx > CMakeFiles/vepl_example1_threshold.dir/example1_threshold.i

contrib/tbl/vepl/examples/CMakeFiles/vepl_example1_threshold.dir/example1_threshold.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/vepl_example1_threshold.dir/example1_threshold.s"
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/contrib/tbl/vepl/examples" && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/antiw/demos/vxl-1.13.0/contrib/tbl/vepl/examples/example1_threshold.cxx -o CMakeFiles/vepl_example1_threshold.dir/example1_threshold.s

contrib/tbl/vepl/examples/CMakeFiles/vepl_example1_threshold.dir/example1_threshold.o.requires:
.PHONY : contrib/tbl/vepl/examples/CMakeFiles/vepl_example1_threshold.dir/example1_threshold.o.requires

contrib/tbl/vepl/examples/CMakeFiles/vepl_example1_threshold.dir/example1_threshold.o.provides: contrib/tbl/vepl/examples/CMakeFiles/vepl_example1_threshold.dir/example1_threshold.o.requires
	$(MAKE) -f contrib/tbl/vepl/examples/CMakeFiles/vepl_example1_threshold.dir/build.make contrib/tbl/vepl/examples/CMakeFiles/vepl_example1_threshold.dir/example1_threshold.o.provides.build
.PHONY : contrib/tbl/vepl/examples/CMakeFiles/vepl_example1_threshold.dir/example1_threshold.o.provides

contrib/tbl/vepl/examples/CMakeFiles/vepl_example1_threshold.dir/example1_threshold.o.provides.build: contrib/tbl/vepl/examples/CMakeFiles/vepl_example1_threshold.dir/example1_threshold.o
.PHONY : contrib/tbl/vepl/examples/CMakeFiles/vepl_example1_threshold.dir/example1_threshold.o.provides.build

# Object files for target vepl_example1_threshold
vepl_example1_threshold_OBJECTS = \
"CMakeFiles/vepl_example1_threshold.dir/example1_threshold.o"

# External object files for target vepl_example1_threshold
vepl_example1_threshold_EXTERNAL_OBJECTS =

contrib/tbl/vepl/examples/vepl_example1_threshold: contrib/tbl/vepl/examples/CMakeFiles/vepl_example1_threshold.dir/example1_threshold.o
contrib/tbl/vepl/examples/vepl_example1_threshold: lib/libvepl.a
contrib/tbl/vepl/examples/vepl_example1_threshold: lib/libvil1.a
contrib/tbl/vepl/examples/vepl_example1_threshold: lib/libvipl.a
contrib/tbl/vepl/examples/vepl_example1_threshold: lib/libvil1.a
contrib/tbl/vepl/examples/vepl_example1_threshold: lib/libvil.a
contrib/tbl/vepl/examples/vepl_example1_threshold: /usr/lib/libjpeg.so
contrib/tbl/vepl/examples/vepl_example1_threshold: /usr/lib/libpng.so
contrib/tbl/vepl/examples/vepl_example1_threshold: /usr/lib/libz.so
contrib/tbl/vepl/examples/vepl_example1_threshold: lib/libgeotiff.a
contrib/tbl/vepl/examples/vepl_example1_threshold: /usr/lib/libtiff.so
contrib/tbl/vepl/examples/vepl_example1_threshold: lib/libvnl.a
contrib/tbl/vepl/examples/vepl_example1_threshold: lib/libvcl.a
contrib/tbl/vepl/examples/vepl_example1_threshold: contrib/tbl/vepl/examples/CMakeFiles/vepl_example1_threshold.dir/build.make
contrib/tbl/vepl/examples/vepl_example1_threshold: contrib/tbl/vepl/examples/CMakeFiles/vepl_example1_threshold.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable vepl_example1_threshold"
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/contrib/tbl/vepl/examples" && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/vepl_example1_threshold.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
contrib/tbl/vepl/examples/CMakeFiles/vepl_example1_threshold.dir/build: contrib/tbl/vepl/examples/vepl_example1_threshold
.PHONY : contrib/tbl/vepl/examples/CMakeFiles/vepl_example1_threshold.dir/build

contrib/tbl/vepl/examples/CMakeFiles/vepl_example1_threshold.dir/requires: contrib/tbl/vepl/examples/CMakeFiles/vepl_example1_threshold.dir/example1_threshold.o.requires
.PHONY : contrib/tbl/vepl/examples/CMakeFiles/vepl_example1_threshold.dir/requires

contrib/tbl/vepl/examples/CMakeFiles/vepl_example1_threshold.dir/clean:
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/contrib/tbl/vepl/examples" && $(CMAKE_COMMAND) -P CMakeFiles/vepl_example1_threshold.dir/cmake_clean.cmake
.PHONY : contrib/tbl/vepl/examples/CMakeFiles/vepl_example1_threshold.dir/clean

contrib/tbl/vepl/examples/CMakeFiles/vepl_example1_threshold.dir/depend:
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/antiw/demos/vxl-1.13.0 /home/antiw/demos/vxl-1.13.0/contrib/tbl/vepl/examples "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin" "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/contrib/tbl/vepl/examples" "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/contrib/tbl/vepl/examples/CMakeFiles/vepl_example1_threshold.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : contrib/tbl/vepl/examples/CMakeFiles/vepl_example1_threshold.dir/depend

