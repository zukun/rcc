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
include contrib/mul/mvl2/CMakeFiles/mvl2.dir/depend.make

# Include the progress variables for this target.
include contrib/mul/mvl2/CMakeFiles/mvl2.dir/progress.make

# Include the compile flags for this target's objects.
include contrib/mul/mvl2/CMakeFiles/mvl2.dir/flags.make

contrib/mul/mvl2/CMakeFiles/mvl2.dir/mvl2_video_reader.o: contrib/mul/mvl2/CMakeFiles/mvl2.dir/flags.make
contrib/mul/mvl2/CMakeFiles/mvl2.dir/mvl2_video_reader.o: /home/antiw/demos/vxl-1.13.0/contrib/mul/mvl2/mvl2_video_reader.cxx
	$(CMAKE_COMMAND) -E cmake_progress_report "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/CMakeFiles" $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object contrib/mul/mvl2/CMakeFiles/mvl2.dir/mvl2_video_reader.o"
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/contrib/mul/mvl2" && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/mvl2.dir/mvl2_video_reader.o -c /home/antiw/demos/vxl-1.13.0/contrib/mul/mvl2/mvl2_video_reader.cxx

contrib/mul/mvl2/CMakeFiles/mvl2.dir/mvl2_video_reader.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/mvl2.dir/mvl2_video_reader.i"
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/contrib/mul/mvl2" && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/antiw/demos/vxl-1.13.0/contrib/mul/mvl2/mvl2_video_reader.cxx > CMakeFiles/mvl2.dir/mvl2_video_reader.i

contrib/mul/mvl2/CMakeFiles/mvl2.dir/mvl2_video_reader.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/mvl2.dir/mvl2_video_reader.s"
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/contrib/mul/mvl2" && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/antiw/demos/vxl-1.13.0/contrib/mul/mvl2/mvl2_video_reader.cxx -o CMakeFiles/mvl2.dir/mvl2_video_reader.s

contrib/mul/mvl2/CMakeFiles/mvl2.dir/mvl2_video_reader.o.requires:
.PHONY : contrib/mul/mvl2/CMakeFiles/mvl2.dir/mvl2_video_reader.o.requires

contrib/mul/mvl2/CMakeFiles/mvl2.dir/mvl2_video_reader.o.provides: contrib/mul/mvl2/CMakeFiles/mvl2.dir/mvl2_video_reader.o.requires
	$(MAKE) -f contrib/mul/mvl2/CMakeFiles/mvl2.dir/build.make contrib/mul/mvl2/CMakeFiles/mvl2.dir/mvl2_video_reader.o.provides.build
.PHONY : contrib/mul/mvl2/CMakeFiles/mvl2.dir/mvl2_video_reader.o.provides

contrib/mul/mvl2/CMakeFiles/mvl2.dir/mvl2_video_reader.o.provides.build: contrib/mul/mvl2/CMakeFiles/mvl2.dir/mvl2_video_reader.o
.PHONY : contrib/mul/mvl2/CMakeFiles/mvl2.dir/mvl2_video_reader.o.provides.build

contrib/mul/mvl2/CMakeFiles/mvl2.dir/mvl2_video_from_sequence.o: contrib/mul/mvl2/CMakeFiles/mvl2.dir/flags.make
contrib/mul/mvl2/CMakeFiles/mvl2.dir/mvl2_video_from_sequence.o: /home/antiw/demos/vxl-1.13.0/contrib/mul/mvl2/mvl2_video_from_sequence.cxx
	$(CMAKE_COMMAND) -E cmake_progress_report "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/CMakeFiles" $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object contrib/mul/mvl2/CMakeFiles/mvl2.dir/mvl2_video_from_sequence.o"
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/contrib/mul/mvl2" && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/mvl2.dir/mvl2_video_from_sequence.o -c /home/antiw/demos/vxl-1.13.0/contrib/mul/mvl2/mvl2_video_from_sequence.cxx

contrib/mul/mvl2/CMakeFiles/mvl2.dir/mvl2_video_from_sequence.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/mvl2.dir/mvl2_video_from_sequence.i"
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/contrib/mul/mvl2" && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/antiw/demos/vxl-1.13.0/contrib/mul/mvl2/mvl2_video_from_sequence.cxx > CMakeFiles/mvl2.dir/mvl2_video_from_sequence.i

contrib/mul/mvl2/CMakeFiles/mvl2.dir/mvl2_video_from_sequence.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/mvl2.dir/mvl2_video_from_sequence.s"
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/contrib/mul/mvl2" && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/antiw/demos/vxl-1.13.0/contrib/mul/mvl2/mvl2_video_from_sequence.cxx -o CMakeFiles/mvl2.dir/mvl2_video_from_sequence.s

contrib/mul/mvl2/CMakeFiles/mvl2.dir/mvl2_video_from_sequence.o.requires:
.PHONY : contrib/mul/mvl2/CMakeFiles/mvl2.dir/mvl2_video_from_sequence.o.requires

contrib/mul/mvl2/CMakeFiles/mvl2.dir/mvl2_video_from_sequence.o.provides: contrib/mul/mvl2/CMakeFiles/mvl2.dir/mvl2_video_from_sequence.o.requires
	$(MAKE) -f contrib/mul/mvl2/CMakeFiles/mvl2.dir/build.make contrib/mul/mvl2/CMakeFiles/mvl2.dir/mvl2_video_from_sequence.o.provides.build
.PHONY : contrib/mul/mvl2/CMakeFiles/mvl2.dir/mvl2_video_from_sequence.o.provides

contrib/mul/mvl2/CMakeFiles/mvl2.dir/mvl2_video_from_sequence.o.provides.build: contrib/mul/mvl2/CMakeFiles/mvl2.dir/mvl2_video_from_sequence.o
.PHONY : contrib/mul/mvl2/CMakeFiles/mvl2.dir/mvl2_video_from_sequence.o.provides.build

# Object files for target mvl2
mvl2_OBJECTS = \
"CMakeFiles/mvl2.dir/mvl2_video_reader.o" \
"CMakeFiles/mvl2.dir/mvl2_video_from_sequence.o"

# External object files for target mvl2
mvl2_EXTERNAL_OBJECTS =

lib/libmvl2.a: contrib/mul/mvl2/CMakeFiles/mvl2.dir/mvl2_video_reader.o
lib/libmvl2.a: contrib/mul/mvl2/CMakeFiles/mvl2.dir/mvl2_video_from_sequence.o
lib/libmvl2.a: contrib/mul/mvl2/CMakeFiles/mvl2.dir/build.make
lib/libmvl2.a: contrib/mul/mvl2/CMakeFiles/mvl2.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX static library ../../../lib/libmvl2.a"
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/contrib/mul/mvl2" && $(CMAKE_COMMAND) -P CMakeFiles/mvl2.dir/cmake_clean_target.cmake
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/contrib/mul/mvl2" && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/mvl2.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
contrib/mul/mvl2/CMakeFiles/mvl2.dir/build: lib/libmvl2.a
.PHONY : contrib/mul/mvl2/CMakeFiles/mvl2.dir/build

contrib/mul/mvl2/CMakeFiles/mvl2.dir/requires: contrib/mul/mvl2/CMakeFiles/mvl2.dir/mvl2_video_reader.o.requires
contrib/mul/mvl2/CMakeFiles/mvl2.dir/requires: contrib/mul/mvl2/CMakeFiles/mvl2.dir/mvl2_video_from_sequence.o.requires
.PHONY : contrib/mul/mvl2/CMakeFiles/mvl2.dir/requires

contrib/mul/mvl2/CMakeFiles/mvl2.dir/clean:
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/contrib/mul/mvl2" && $(CMAKE_COMMAND) -P CMakeFiles/mvl2.dir/cmake_clean.cmake
.PHONY : contrib/mul/mvl2/CMakeFiles/mvl2.dir/clean

contrib/mul/mvl2/CMakeFiles/mvl2.dir/depend:
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/antiw/demos/vxl-1.13.0 /home/antiw/demos/vxl-1.13.0/contrib/mul/mvl2 "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin" "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/contrib/mul/mvl2" "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/contrib/mul/mvl2/CMakeFiles/mvl2.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : contrib/mul/mvl2/CMakeFiles/mvl2.dir/depend

