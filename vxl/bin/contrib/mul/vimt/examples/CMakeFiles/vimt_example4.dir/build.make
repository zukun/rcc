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
include contrib/mul/vimt/examples/CMakeFiles/vimt_example4.dir/depend.make

# Include the progress variables for this target.
include contrib/mul/vimt/examples/CMakeFiles/vimt_example4.dir/progress.make

# Include the compile flags for this target's objects.
include contrib/mul/vimt/examples/CMakeFiles/vimt_example4.dir/flags.make

contrib/mul/vimt/examples/CMakeFiles/vimt_example4.dir/example4.o: contrib/mul/vimt/examples/CMakeFiles/vimt_example4.dir/flags.make
contrib/mul/vimt/examples/CMakeFiles/vimt_example4.dir/example4.o: /home/antiw/demos/vxl-1.13.0/contrib/mul/vimt/examples/example4.cxx
	$(CMAKE_COMMAND) -E cmake_progress_report "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/CMakeFiles" $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object contrib/mul/vimt/examples/CMakeFiles/vimt_example4.dir/example4.o"
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/contrib/mul/vimt/examples" && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/vimt_example4.dir/example4.o -c /home/antiw/demos/vxl-1.13.0/contrib/mul/vimt/examples/example4.cxx

contrib/mul/vimt/examples/CMakeFiles/vimt_example4.dir/example4.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/vimt_example4.dir/example4.i"
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/contrib/mul/vimt/examples" && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/antiw/demos/vxl-1.13.0/contrib/mul/vimt/examples/example4.cxx > CMakeFiles/vimt_example4.dir/example4.i

contrib/mul/vimt/examples/CMakeFiles/vimt_example4.dir/example4.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/vimt_example4.dir/example4.s"
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/contrib/mul/vimt/examples" && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/antiw/demos/vxl-1.13.0/contrib/mul/vimt/examples/example4.cxx -o CMakeFiles/vimt_example4.dir/example4.s

contrib/mul/vimt/examples/CMakeFiles/vimt_example4.dir/example4.o.requires:
.PHONY : contrib/mul/vimt/examples/CMakeFiles/vimt_example4.dir/example4.o.requires

contrib/mul/vimt/examples/CMakeFiles/vimt_example4.dir/example4.o.provides: contrib/mul/vimt/examples/CMakeFiles/vimt_example4.dir/example4.o.requires
	$(MAKE) -f contrib/mul/vimt/examples/CMakeFiles/vimt_example4.dir/build.make contrib/mul/vimt/examples/CMakeFiles/vimt_example4.dir/example4.o.provides.build
.PHONY : contrib/mul/vimt/examples/CMakeFiles/vimt_example4.dir/example4.o.provides

contrib/mul/vimt/examples/CMakeFiles/vimt_example4.dir/example4.o.provides.build: contrib/mul/vimt/examples/CMakeFiles/vimt_example4.dir/example4.o
.PHONY : contrib/mul/vimt/examples/CMakeFiles/vimt_example4.dir/example4.o.provides.build

# Object files for target vimt_example4
vimt_example4_OBJECTS = \
"CMakeFiles/vimt_example4.dir/example4.o"

# External object files for target vimt_example4
vimt_example4_EXTERNAL_OBJECTS =

contrib/mul/vimt/examples/vimt_example4: contrib/mul/vimt/examples/CMakeFiles/vimt_example4.dir/example4.o
contrib/mul/vimt/examples/vimt_example4: lib/libvimt_algo.a
contrib/mul/vimt/examples/vimt_example4: lib/libvimt.a
contrib/mul/vimt/examples/vimt_example4: lib/libvnl.a
contrib/mul/vimt/examples/vimt_example4: lib/libvil.a
contrib/mul/vimt/examples/vimt_example4: lib/libvul.a
contrib/mul/vimt/examples/vimt_example4: lib/libmbl.a
contrib/mul/vimt/examples/vimt_example4: lib/libvul.a
contrib/mul/vimt/examples/vimt_example4: lib/libvnl_io.a
contrib/mul/vimt/examples/vimt_example4: lib/libvgl_io.a
contrib/mul/vimt/examples/vimt_example4: lib/libvgl.a
contrib/mul/vimt/examples/vimt_example4: lib/libvbl_io.a
contrib/mul/vimt/examples/vimt_example4: lib/libvbl.a
contrib/mul/vimt/examples/vimt_example4: lib/libvil_io.a
contrib/mul/vimt/examples/vimt_example4: lib/libvsl.a
contrib/mul/vimt/examples/vimt_example4: lib/libvil_algo.a
contrib/mul/vimt/examples/vimt_example4: lib/libvil.a
contrib/mul/vimt/examples/vimt_example4: /usr/lib/libjpeg.so
contrib/mul/vimt/examples/vimt_example4: lib/libgeotiff.a
contrib/mul/vimt/examples/vimt_example4: /usr/lib/libtiff.so
contrib/mul/vimt/examples/vimt_example4: /usr/lib/libpng.so
contrib/mul/vimt/examples/vimt_example4: /usr/lib/libz.so
contrib/mul/vimt/examples/vimt_example4: lib/libvnl_algo.a
contrib/mul/vimt/examples/vimt_example4: lib/libvnl.a
contrib/mul/vimt/examples/vimt_example4: lib/libnetlib.a
contrib/mul/vimt/examples/vimt_example4: lib/libv3p_netlib.a
contrib/mul/vimt/examples/vimt_example4: lib/libvcl.a
contrib/mul/vimt/examples/vimt_example4: contrib/mul/vimt/examples/CMakeFiles/vimt_example4.dir/build.make
contrib/mul/vimt/examples/vimt_example4: contrib/mul/vimt/examples/CMakeFiles/vimt_example4.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable vimt_example4"
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/contrib/mul/vimt/examples" && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/vimt_example4.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
contrib/mul/vimt/examples/CMakeFiles/vimt_example4.dir/build: contrib/mul/vimt/examples/vimt_example4
.PHONY : contrib/mul/vimt/examples/CMakeFiles/vimt_example4.dir/build

contrib/mul/vimt/examples/CMakeFiles/vimt_example4.dir/requires: contrib/mul/vimt/examples/CMakeFiles/vimt_example4.dir/example4.o.requires
.PHONY : contrib/mul/vimt/examples/CMakeFiles/vimt_example4.dir/requires

contrib/mul/vimt/examples/CMakeFiles/vimt_example4.dir/clean:
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/contrib/mul/vimt/examples" && $(CMAKE_COMMAND) -P CMakeFiles/vimt_example4.dir/cmake_clean.cmake
.PHONY : contrib/mul/vimt/examples/CMakeFiles/vimt_example4.dir/clean

contrib/mul/vimt/examples/CMakeFiles/vimt_example4.dir/depend:
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/antiw/demos/vxl-1.13.0 /home/antiw/demos/vxl-1.13.0/contrib/mul/vimt/examples "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin" "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/contrib/mul/vimt/examples" "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/contrib/mul/vimt/examples/CMakeFiles/vimt_example4.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : contrib/mul/vimt/examples/CMakeFiles/vimt_example4.dir/depend

