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
include contrib/rpl/rgrl/examples/CMakeFiles/registration_range_data.dir/depend.make

# Include the progress variables for this target.
include contrib/rpl/rgrl/examples/CMakeFiles/registration_range_data.dir/progress.make

# Include the compile flags for this target's objects.
include contrib/rpl/rgrl/examples/CMakeFiles/registration_range_data.dir/flags.make

contrib/rpl/rgrl/examples/CMakeFiles/registration_range_data.dir/registration_range_data.o: contrib/rpl/rgrl/examples/CMakeFiles/registration_range_data.dir/flags.make
contrib/rpl/rgrl/examples/CMakeFiles/registration_range_data.dir/registration_range_data.o: /home/antiw/demos/vxl-1.13.0/contrib/rpl/rgrl/examples/registration_range_data.cxx
	$(CMAKE_COMMAND) -E cmake_progress_report "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/CMakeFiles" $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object contrib/rpl/rgrl/examples/CMakeFiles/registration_range_data.dir/registration_range_data.o"
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/contrib/rpl/rgrl/examples" && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/registration_range_data.dir/registration_range_data.o -c /home/antiw/demos/vxl-1.13.0/contrib/rpl/rgrl/examples/registration_range_data.cxx

contrib/rpl/rgrl/examples/CMakeFiles/registration_range_data.dir/registration_range_data.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/registration_range_data.dir/registration_range_data.i"
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/contrib/rpl/rgrl/examples" && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/antiw/demos/vxl-1.13.0/contrib/rpl/rgrl/examples/registration_range_data.cxx > CMakeFiles/registration_range_data.dir/registration_range_data.i

contrib/rpl/rgrl/examples/CMakeFiles/registration_range_data.dir/registration_range_data.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/registration_range_data.dir/registration_range_data.s"
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/contrib/rpl/rgrl/examples" && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/antiw/demos/vxl-1.13.0/contrib/rpl/rgrl/examples/registration_range_data.cxx -o CMakeFiles/registration_range_data.dir/registration_range_data.s

contrib/rpl/rgrl/examples/CMakeFiles/registration_range_data.dir/registration_range_data.o.requires:
.PHONY : contrib/rpl/rgrl/examples/CMakeFiles/registration_range_data.dir/registration_range_data.o.requires

contrib/rpl/rgrl/examples/CMakeFiles/registration_range_data.dir/registration_range_data.o.provides: contrib/rpl/rgrl/examples/CMakeFiles/registration_range_data.dir/registration_range_data.o.requires
	$(MAKE) -f contrib/rpl/rgrl/examples/CMakeFiles/registration_range_data.dir/build.make contrib/rpl/rgrl/examples/CMakeFiles/registration_range_data.dir/registration_range_data.o.provides.build
.PHONY : contrib/rpl/rgrl/examples/CMakeFiles/registration_range_data.dir/registration_range_data.o.provides

contrib/rpl/rgrl/examples/CMakeFiles/registration_range_data.dir/registration_range_data.o.provides.build: contrib/rpl/rgrl/examples/CMakeFiles/registration_range_data.dir/registration_range_data.o
.PHONY : contrib/rpl/rgrl/examples/CMakeFiles/registration_range_data.dir/registration_range_data.o.provides.build

# Object files for target registration_range_data
registration_range_data_OBJECTS = \
"CMakeFiles/registration_range_data.dir/registration_range_data.o"

# External object files for target registration_range_data
registration_range_data_EXTERNAL_OBJECTS =

contrib/rpl/rgrl/examples/registration_range_data: contrib/rpl/rgrl/examples/CMakeFiles/registration_range_data.dir/registration_range_data.o
contrib/rpl/rgrl/examples/registration_range_data: lib/librgrl.a
contrib/rpl/rgrl/examples/registration_range_data: lib/libtestlib.a
contrib/rpl/rgrl/examples/registration_range_data: lib/librrel.a
contrib/rpl/rgrl/examples/registration_range_data: lib/libvgl.a
contrib/rpl/rgrl/examples/registration_range_data: lib/librsdl.a
contrib/rpl/rgrl/examples/registration_range_data: lib/libvil3d.a
contrib/rpl/rgrl/examples/registration_range_data: lib/libvsl.a
contrib/rpl/rgrl/examples/registration_range_data: lib/libvil.a
contrib/rpl/rgrl/examples/registration_range_data: /usr/lib/libjpeg.so
contrib/rpl/rgrl/examples/registration_range_data: lib/libgeotiff.a
contrib/rpl/rgrl/examples/registration_range_data: /usr/lib/libtiff.so
contrib/rpl/rgrl/examples/registration_range_data: /usr/lib/libpng.so
contrib/rpl/rgrl/examples/registration_range_data: /usr/lib/libz.so
contrib/rpl/rgrl/examples/registration_range_data: lib/libvnl_algo.a
contrib/rpl/rgrl/examples/registration_range_data: lib/libnetlib.a
contrib/rpl/rgrl/examples/registration_range_data: lib/libv3p_netlib.a
contrib/rpl/rgrl/examples/registration_range_data: lib/libvnl.a
contrib/rpl/rgrl/examples/registration_range_data: lib/libvbl.a
contrib/rpl/rgrl/examples/registration_range_data: lib/libvul.a
contrib/rpl/rgrl/examples/registration_range_data: lib/libvcl.a
contrib/rpl/rgrl/examples/registration_range_data: contrib/rpl/rgrl/examples/CMakeFiles/registration_range_data.dir/build.make
contrib/rpl/rgrl/examples/registration_range_data: contrib/rpl/rgrl/examples/CMakeFiles/registration_range_data.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable registration_range_data"
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/contrib/rpl/rgrl/examples" && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/registration_range_data.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
contrib/rpl/rgrl/examples/CMakeFiles/registration_range_data.dir/build: contrib/rpl/rgrl/examples/registration_range_data
.PHONY : contrib/rpl/rgrl/examples/CMakeFiles/registration_range_data.dir/build

contrib/rpl/rgrl/examples/CMakeFiles/registration_range_data.dir/requires: contrib/rpl/rgrl/examples/CMakeFiles/registration_range_data.dir/registration_range_data.o.requires
.PHONY : contrib/rpl/rgrl/examples/CMakeFiles/registration_range_data.dir/requires

contrib/rpl/rgrl/examples/CMakeFiles/registration_range_data.dir/clean:
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/contrib/rpl/rgrl/examples" && $(CMAKE_COMMAND) -P CMakeFiles/registration_range_data.dir/cmake_clean.cmake
.PHONY : contrib/rpl/rgrl/examples/CMakeFiles/registration_range_data.dir/clean

contrib/rpl/rgrl/examples/CMakeFiles/registration_range_data.dir/depend:
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/antiw/demos/vxl-1.13.0 /home/antiw/demos/vxl-1.13.0/contrib/rpl/rgrl/examples "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin" "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/contrib/rpl/rgrl/examples" "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/contrib/rpl/rgrl/examples/CMakeFiles/registration_range_data.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : contrib/rpl/rgrl/examples/CMakeFiles/registration_range_data.dir/depend

