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
include contrib/tbl/vipl/examples/CMakeFiles/vipl_example_convert.dir/depend.make

# Include the progress variables for this target.
include contrib/tbl/vipl/examples/CMakeFiles/vipl_example_convert.dir/progress.make

# Include the compile flags for this target's objects.
include contrib/tbl/vipl/examples/CMakeFiles/vipl_example_convert.dir/flags.make

contrib/tbl/vipl/examples/CMakeFiles/vipl_example_convert.dir/example_convert.o: contrib/tbl/vipl/examples/CMakeFiles/vipl_example_convert.dir/flags.make
contrib/tbl/vipl/examples/CMakeFiles/vipl_example_convert.dir/example_convert.o: /home/antiw/demos/vxl-1.13.0/contrib/tbl/vipl/examples/example_convert.cxx
	$(CMAKE_COMMAND) -E cmake_progress_report "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/CMakeFiles" $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object contrib/tbl/vipl/examples/CMakeFiles/vipl_example_convert.dir/example_convert.o"
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/contrib/tbl/vipl/examples" && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/vipl_example_convert.dir/example_convert.o -c /home/antiw/demos/vxl-1.13.0/contrib/tbl/vipl/examples/example_convert.cxx

contrib/tbl/vipl/examples/CMakeFiles/vipl_example_convert.dir/example_convert.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/vipl_example_convert.dir/example_convert.i"
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/contrib/tbl/vipl/examples" && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/antiw/demos/vxl-1.13.0/contrib/tbl/vipl/examples/example_convert.cxx > CMakeFiles/vipl_example_convert.dir/example_convert.i

contrib/tbl/vipl/examples/CMakeFiles/vipl_example_convert.dir/example_convert.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/vipl_example_convert.dir/example_convert.s"
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/contrib/tbl/vipl/examples" && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/antiw/demos/vxl-1.13.0/contrib/tbl/vipl/examples/example_convert.cxx -o CMakeFiles/vipl_example_convert.dir/example_convert.s

contrib/tbl/vipl/examples/CMakeFiles/vipl_example_convert.dir/example_convert.o.requires:
.PHONY : contrib/tbl/vipl/examples/CMakeFiles/vipl_example_convert.dir/example_convert.o.requires

contrib/tbl/vipl/examples/CMakeFiles/vipl_example_convert.dir/example_convert.o.provides: contrib/tbl/vipl/examples/CMakeFiles/vipl_example_convert.dir/example_convert.o.requires
	$(MAKE) -f contrib/tbl/vipl/examples/CMakeFiles/vipl_example_convert.dir/build.make contrib/tbl/vipl/examples/CMakeFiles/vipl_example_convert.dir/example_convert.o.provides.build
.PHONY : contrib/tbl/vipl/examples/CMakeFiles/vipl_example_convert.dir/example_convert.o.provides

contrib/tbl/vipl/examples/CMakeFiles/vipl_example_convert.dir/example_convert.o.provides.build: contrib/tbl/vipl/examples/CMakeFiles/vipl_example_convert.dir/example_convert.o
.PHONY : contrib/tbl/vipl/examples/CMakeFiles/vipl_example_convert.dir/example_convert.o.provides.build

# Object files for target vipl_example_convert
vipl_example_convert_OBJECTS = \
"CMakeFiles/vipl_example_convert.dir/example_convert.o"

# External object files for target vipl_example_convert
vipl_example_convert_EXTERNAL_OBJECTS =

contrib/tbl/vipl/examples/vipl_example_convert: contrib/tbl/vipl/examples/CMakeFiles/vipl_example_convert.dir/example_convert.o
contrib/tbl/vipl/examples/vipl_example_convert: lib/libvipl.a
contrib/tbl/vipl/examples/vipl_example_convert: lib/libvil.a
contrib/tbl/vipl/examples/vipl_example_convert: lib/libgeotiff.a
contrib/tbl/vipl/examples/vipl_example_convert: lib/libvil1.a
contrib/tbl/vipl/examples/vipl_example_convert: /usr/lib/libjpeg.so
contrib/tbl/vipl/examples/vipl_example_convert: /usr/lib/libtiff.so
contrib/tbl/vipl/examples/vipl_example_convert: /usr/lib/libpng.so
contrib/tbl/vipl/examples/vipl_example_convert: /usr/lib/libz.so
contrib/tbl/vipl/examples/vipl_example_convert: lib/libvnl.a
contrib/tbl/vipl/examples/vipl_example_convert: lib/libvcl.a
contrib/tbl/vipl/examples/vipl_example_convert: contrib/tbl/vipl/examples/CMakeFiles/vipl_example_convert.dir/build.make
contrib/tbl/vipl/examples/vipl_example_convert: contrib/tbl/vipl/examples/CMakeFiles/vipl_example_convert.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable vipl_example_convert"
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/contrib/tbl/vipl/examples" && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/vipl_example_convert.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
contrib/tbl/vipl/examples/CMakeFiles/vipl_example_convert.dir/build: contrib/tbl/vipl/examples/vipl_example_convert
.PHONY : contrib/tbl/vipl/examples/CMakeFiles/vipl_example_convert.dir/build

contrib/tbl/vipl/examples/CMakeFiles/vipl_example_convert.dir/requires: contrib/tbl/vipl/examples/CMakeFiles/vipl_example_convert.dir/example_convert.o.requires
.PHONY : contrib/tbl/vipl/examples/CMakeFiles/vipl_example_convert.dir/requires

contrib/tbl/vipl/examples/CMakeFiles/vipl_example_convert.dir/clean:
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/contrib/tbl/vipl/examples" && $(CMAKE_COMMAND) -P CMakeFiles/vipl_example_convert.dir/cmake_clean.cmake
.PHONY : contrib/tbl/vipl/examples/CMakeFiles/vipl_example_convert.dir/clean

contrib/tbl/vipl/examples/CMakeFiles/vipl_example_convert.dir/depend:
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/antiw/demos/vxl-1.13.0 /home/antiw/demos/vxl-1.13.0/contrib/tbl/vipl/examples "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin" "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/contrib/tbl/vipl/examples" "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/contrib/tbl/vipl/examples/CMakeFiles/vipl_example_convert.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : contrib/tbl/vipl/examples/CMakeFiles/vipl_example_convert.dir/depend

