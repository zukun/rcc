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
include contrib/tbl/vepl2/examples/CMakeFiles/vepl2_example_std_dev.dir/depend.make

# Include the progress variables for this target.
include contrib/tbl/vepl2/examples/CMakeFiles/vepl2_example_std_dev.dir/progress.make

# Include the compile flags for this target's objects.
include contrib/tbl/vepl2/examples/CMakeFiles/vepl2_example_std_dev.dir/flags.make

contrib/tbl/vepl2/examples/CMakeFiles/vepl2_example_std_dev.dir/example_std_dev.o: contrib/tbl/vepl2/examples/CMakeFiles/vepl2_example_std_dev.dir/flags.make
contrib/tbl/vepl2/examples/CMakeFiles/vepl2_example_std_dev.dir/example_std_dev.o: /home/antiw/demos/vxl-1.13.0/contrib/tbl/vepl2/examples/example_std_dev.cxx
	$(CMAKE_COMMAND) -E cmake_progress_report "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/CMakeFiles" $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object contrib/tbl/vepl2/examples/CMakeFiles/vepl2_example_std_dev.dir/example_std_dev.o"
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/contrib/tbl/vepl2/examples" && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -O0 -o CMakeFiles/vepl2_example_std_dev.dir/example_std_dev.o -c /home/antiw/demos/vxl-1.13.0/contrib/tbl/vepl2/examples/example_std_dev.cxx

contrib/tbl/vepl2/examples/CMakeFiles/vepl2_example_std_dev.dir/example_std_dev.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/vepl2_example_std_dev.dir/example_std_dev.i"
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/contrib/tbl/vepl2/examples" && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -O0 -E /home/antiw/demos/vxl-1.13.0/contrib/tbl/vepl2/examples/example_std_dev.cxx > CMakeFiles/vepl2_example_std_dev.dir/example_std_dev.i

contrib/tbl/vepl2/examples/CMakeFiles/vepl2_example_std_dev.dir/example_std_dev.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/vepl2_example_std_dev.dir/example_std_dev.s"
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/contrib/tbl/vepl2/examples" && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -O0 -S /home/antiw/demos/vxl-1.13.0/contrib/tbl/vepl2/examples/example_std_dev.cxx -o CMakeFiles/vepl2_example_std_dev.dir/example_std_dev.s

contrib/tbl/vepl2/examples/CMakeFiles/vepl2_example_std_dev.dir/example_std_dev.o.requires:
.PHONY : contrib/tbl/vepl2/examples/CMakeFiles/vepl2_example_std_dev.dir/example_std_dev.o.requires

contrib/tbl/vepl2/examples/CMakeFiles/vepl2_example_std_dev.dir/example_std_dev.o.provides: contrib/tbl/vepl2/examples/CMakeFiles/vepl2_example_std_dev.dir/example_std_dev.o.requires
	$(MAKE) -f contrib/tbl/vepl2/examples/CMakeFiles/vepl2_example_std_dev.dir/build.make contrib/tbl/vepl2/examples/CMakeFiles/vepl2_example_std_dev.dir/example_std_dev.o.provides.build
.PHONY : contrib/tbl/vepl2/examples/CMakeFiles/vepl2_example_std_dev.dir/example_std_dev.o.provides

contrib/tbl/vepl2/examples/CMakeFiles/vepl2_example_std_dev.dir/example_std_dev.o.provides.build: contrib/tbl/vepl2/examples/CMakeFiles/vepl2_example_std_dev.dir/example_std_dev.o
.PHONY : contrib/tbl/vepl2/examples/CMakeFiles/vepl2_example_std_dev.dir/example_std_dev.o.provides.build

# Object files for target vepl2_example_std_dev
vepl2_example_std_dev_OBJECTS = \
"CMakeFiles/vepl2_example_std_dev.dir/example_std_dev.o"

# External object files for target vepl2_example_std_dev
vepl2_example_std_dev_EXTERNAL_OBJECTS =

contrib/tbl/vepl2/examples/vepl2_example_std_dev: contrib/tbl/vepl2/examples/CMakeFiles/vepl2_example_std_dev.dir/example_std_dev.o
contrib/tbl/vepl2/examples/vepl2_example_std_dev: lib/libvepl2.a
contrib/tbl/vepl2/examples/vepl2_example_std_dev: lib/libvil.a
contrib/tbl/vepl2/examples/vepl2_example_std_dev: lib/libvipl.a
contrib/tbl/vepl2/examples/vepl2_example_std_dev: lib/libvil.a
contrib/tbl/vepl2/examples/vepl2_example_std_dev: lib/libgeotiff.a
contrib/tbl/vepl2/examples/vepl2_example_std_dev: lib/libvil1.a
contrib/tbl/vepl2/examples/vepl2_example_std_dev: /usr/lib/libjpeg.so
contrib/tbl/vepl2/examples/vepl2_example_std_dev: /usr/lib/libtiff.so
contrib/tbl/vepl2/examples/vepl2_example_std_dev: /usr/lib/libpng.so
contrib/tbl/vepl2/examples/vepl2_example_std_dev: /usr/lib/libz.so
contrib/tbl/vepl2/examples/vepl2_example_std_dev: lib/libvnl.a
contrib/tbl/vepl2/examples/vepl2_example_std_dev: lib/libvcl.a
contrib/tbl/vepl2/examples/vepl2_example_std_dev: contrib/tbl/vepl2/examples/CMakeFiles/vepl2_example_std_dev.dir/build.make
contrib/tbl/vepl2/examples/vepl2_example_std_dev: contrib/tbl/vepl2/examples/CMakeFiles/vepl2_example_std_dev.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable vepl2_example_std_dev"
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/contrib/tbl/vepl2/examples" && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/vepl2_example_std_dev.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
contrib/tbl/vepl2/examples/CMakeFiles/vepl2_example_std_dev.dir/build: contrib/tbl/vepl2/examples/vepl2_example_std_dev
.PHONY : contrib/tbl/vepl2/examples/CMakeFiles/vepl2_example_std_dev.dir/build

contrib/tbl/vepl2/examples/CMakeFiles/vepl2_example_std_dev.dir/requires: contrib/tbl/vepl2/examples/CMakeFiles/vepl2_example_std_dev.dir/example_std_dev.o.requires
.PHONY : contrib/tbl/vepl2/examples/CMakeFiles/vepl2_example_std_dev.dir/requires

contrib/tbl/vepl2/examples/CMakeFiles/vepl2_example_std_dev.dir/clean:
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/contrib/tbl/vepl2/examples" && $(CMAKE_COMMAND) -P CMakeFiles/vepl2_example_std_dev.dir/cmake_clean.cmake
.PHONY : contrib/tbl/vepl2/examples/CMakeFiles/vepl2_example_std_dev.dir/clean

contrib/tbl/vepl2/examples/CMakeFiles/vepl2_example_std_dev.dir/depend:
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/antiw/demos/vxl-1.13.0 /home/antiw/demos/vxl-1.13.0/contrib/tbl/vepl2/examples "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin" "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/contrib/tbl/vepl2/examples" "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/contrib/tbl/vepl2/examples/CMakeFiles/vepl2_example_std_dev.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : contrib/tbl/vepl2/examples/CMakeFiles/vepl2_example_std_dev.dir/depend

