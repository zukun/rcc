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
include contrib/rpl/rgrl/examples/CMakeFiles/registration3.dir/depend.make

# Include the progress variables for this target.
include contrib/rpl/rgrl/examples/CMakeFiles/registration3.dir/progress.make

# Include the compile flags for this target's objects.
include contrib/rpl/rgrl/examples/CMakeFiles/registration3.dir/flags.make

contrib/rpl/rgrl/examples/CMakeFiles/registration3.dir/registration3.o: contrib/rpl/rgrl/examples/CMakeFiles/registration3.dir/flags.make
contrib/rpl/rgrl/examples/CMakeFiles/registration3.dir/registration3.o: /home/antiw/demos/vxl-1.13.0/contrib/rpl/rgrl/examples/registration3.cxx
	$(CMAKE_COMMAND) -E cmake_progress_report "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/CMakeFiles" $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object contrib/rpl/rgrl/examples/CMakeFiles/registration3.dir/registration3.o"
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/contrib/rpl/rgrl/examples" && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/registration3.dir/registration3.o -c /home/antiw/demos/vxl-1.13.0/contrib/rpl/rgrl/examples/registration3.cxx

contrib/rpl/rgrl/examples/CMakeFiles/registration3.dir/registration3.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/registration3.dir/registration3.i"
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/contrib/rpl/rgrl/examples" && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/antiw/demos/vxl-1.13.0/contrib/rpl/rgrl/examples/registration3.cxx > CMakeFiles/registration3.dir/registration3.i

contrib/rpl/rgrl/examples/CMakeFiles/registration3.dir/registration3.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/registration3.dir/registration3.s"
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/contrib/rpl/rgrl/examples" && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/antiw/demos/vxl-1.13.0/contrib/rpl/rgrl/examples/registration3.cxx -o CMakeFiles/registration3.dir/registration3.s

contrib/rpl/rgrl/examples/CMakeFiles/registration3.dir/registration3.o.requires:
.PHONY : contrib/rpl/rgrl/examples/CMakeFiles/registration3.dir/registration3.o.requires

contrib/rpl/rgrl/examples/CMakeFiles/registration3.dir/registration3.o.provides: contrib/rpl/rgrl/examples/CMakeFiles/registration3.dir/registration3.o.requires
	$(MAKE) -f contrib/rpl/rgrl/examples/CMakeFiles/registration3.dir/build.make contrib/rpl/rgrl/examples/CMakeFiles/registration3.dir/registration3.o.provides.build
.PHONY : contrib/rpl/rgrl/examples/CMakeFiles/registration3.dir/registration3.o.provides

contrib/rpl/rgrl/examples/CMakeFiles/registration3.dir/registration3.o.provides.build: contrib/rpl/rgrl/examples/CMakeFiles/registration3.dir/registration3.o
.PHONY : contrib/rpl/rgrl/examples/CMakeFiles/registration3.dir/registration3.o.provides.build

# Object files for target registration3
registration3_OBJECTS = \
"CMakeFiles/registration3.dir/registration3.o"

# External object files for target registration3
registration3_EXTERNAL_OBJECTS =

contrib/rpl/rgrl/examples/registration3: contrib/rpl/rgrl/examples/CMakeFiles/registration3.dir/registration3.o
contrib/rpl/rgrl/examples/registration3: lib/librgrl.a
contrib/rpl/rgrl/examples/registration3: lib/libtestlib.a
contrib/rpl/rgrl/examples/registration3: lib/librrel.a
contrib/rpl/rgrl/examples/registration3: lib/libvgl.a
contrib/rpl/rgrl/examples/registration3: lib/librsdl.a
contrib/rpl/rgrl/examples/registration3: lib/libvil3d.a
contrib/rpl/rgrl/examples/registration3: lib/libvsl.a
contrib/rpl/rgrl/examples/registration3: lib/libvil.a
contrib/rpl/rgrl/examples/registration3: /usr/lib/libjpeg.so
contrib/rpl/rgrl/examples/registration3: lib/libgeotiff.a
contrib/rpl/rgrl/examples/registration3: /usr/lib/libtiff.so
contrib/rpl/rgrl/examples/registration3: /usr/lib/libpng.so
contrib/rpl/rgrl/examples/registration3: /usr/lib/libz.so
contrib/rpl/rgrl/examples/registration3: lib/libvnl_algo.a
contrib/rpl/rgrl/examples/registration3: lib/libnetlib.a
contrib/rpl/rgrl/examples/registration3: lib/libv3p_netlib.a
contrib/rpl/rgrl/examples/registration3: lib/libvnl.a
contrib/rpl/rgrl/examples/registration3: lib/libvbl.a
contrib/rpl/rgrl/examples/registration3: lib/libvul.a
contrib/rpl/rgrl/examples/registration3: lib/libvcl.a
contrib/rpl/rgrl/examples/registration3: contrib/rpl/rgrl/examples/CMakeFiles/registration3.dir/build.make
contrib/rpl/rgrl/examples/registration3: contrib/rpl/rgrl/examples/CMakeFiles/registration3.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable registration3"
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/contrib/rpl/rgrl/examples" && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/registration3.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
contrib/rpl/rgrl/examples/CMakeFiles/registration3.dir/build: contrib/rpl/rgrl/examples/registration3
.PHONY : contrib/rpl/rgrl/examples/CMakeFiles/registration3.dir/build

contrib/rpl/rgrl/examples/CMakeFiles/registration3.dir/requires: contrib/rpl/rgrl/examples/CMakeFiles/registration3.dir/registration3.o.requires
.PHONY : contrib/rpl/rgrl/examples/CMakeFiles/registration3.dir/requires

contrib/rpl/rgrl/examples/CMakeFiles/registration3.dir/clean:
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/contrib/rpl/rgrl/examples" && $(CMAKE_COMMAND) -P CMakeFiles/registration3.dir/cmake_clean.cmake
.PHONY : contrib/rpl/rgrl/examples/CMakeFiles/registration3.dir/clean

contrib/rpl/rgrl/examples/CMakeFiles/registration3.dir/depend:
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/antiw/demos/vxl-1.13.0 /home/antiw/demos/vxl-1.13.0/contrib/rpl/rgrl/examples "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin" "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/contrib/rpl/rgrl/examples" "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/contrib/rpl/rgrl/examples/CMakeFiles/registration3.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : contrib/rpl/rgrl/examples/CMakeFiles/registration3.dir/depend

