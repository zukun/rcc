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
include core/tests/CMakeFiles/core_test_all.dir/depend.make

# Include the progress variables for this target.
include core/tests/CMakeFiles/core_test_all.dir/progress.make

# Include the compile flags for this target's objects.
include core/tests/CMakeFiles/core_test_all.dir/flags.make

core/tests/CMakeFiles/core_test_all.dir/test_driver.o: core/tests/CMakeFiles/core_test_all.dir/flags.make
core/tests/CMakeFiles/core_test_all.dir/test_driver.o: /home/antiw/demos/vxl-1.13.0/core/tests/test_driver.cxx
	$(CMAKE_COMMAND) -E cmake_progress_report "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/CMakeFiles" $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object core/tests/CMakeFiles/core_test_all.dir/test_driver.o"
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/core/tests" && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/core_test_all.dir/test_driver.o -c /home/antiw/demos/vxl-1.13.0/core/tests/test_driver.cxx

core/tests/CMakeFiles/core_test_all.dir/test_driver.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/core_test_all.dir/test_driver.i"
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/core/tests" && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/antiw/demos/vxl-1.13.0/core/tests/test_driver.cxx > CMakeFiles/core_test_all.dir/test_driver.i

core/tests/CMakeFiles/core_test_all.dir/test_driver.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/core_test_all.dir/test_driver.s"
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/core/tests" && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/antiw/demos/vxl-1.13.0/core/tests/test_driver.cxx -o CMakeFiles/core_test_all.dir/test_driver.s

core/tests/CMakeFiles/core_test_all.dir/test_driver.o.requires:
.PHONY : core/tests/CMakeFiles/core_test_all.dir/test_driver.o.requires

core/tests/CMakeFiles/core_test_all.dir/test_driver.o.provides: core/tests/CMakeFiles/core_test_all.dir/test_driver.o.requires
	$(MAKE) -f core/tests/CMakeFiles/core_test_all.dir/build.make core/tests/CMakeFiles/core_test_all.dir/test_driver.o.provides.build
.PHONY : core/tests/CMakeFiles/core_test_all.dir/test_driver.o.provides

core/tests/CMakeFiles/core_test_all.dir/test_driver.o.provides.build: core/tests/CMakeFiles/core_test_all.dir/test_driver.o
.PHONY : core/tests/CMakeFiles/core_test_all.dir/test_driver.o.provides.build

core/tests/CMakeFiles/core_test_all.dir/test_config.o: core/tests/CMakeFiles/core_test_all.dir/flags.make
core/tests/CMakeFiles/core_test_all.dir/test_config.o: /home/antiw/demos/vxl-1.13.0/core/tests/test_config.cxx
	$(CMAKE_COMMAND) -E cmake_progress_report "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/CMakeFiles" $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object core/tests/CMakeFiles/core_test_all.dir/test_config.o"
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/core/tests" && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/core_test_all.dir/test_config.o -c /home/antiw/demos/vxl-1.13.0/core/tests/test_config.cxx

core/tests/CMakeFiles/core_test_all.dir/test_config.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/core_test_all.dir/test_config.i"
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/core/tests" && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/antiw/demos/vxl-1.13.0/core/tests/test_config.cxx > CMakeFiles/core_test_all.dir/test_config.i

core/tests/CMakeFiles/core_test_all.dir/test_config.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/core_test_all.dir/test_config.s"
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/core/tests" && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/antiw/demos/vxl-1.13.0/core/tests/test_config.cxx -o CMakeFiles/core_test_all.dir/test_config.s

core/tests/CMakeFiles/core_test_all.dir/test_config.o.requires:
.PHONY : core/tests/CMakeFiles/core_test_all.dir/test_config.o.requires

core/tests/CMakeFiles/core_test_all.dir/test_config.o.provides: core/tests/CMakeFiles/core_test_all.dir/test_config.o.requires
	$(MAKE) -f core/tests/CMakeFiles/core_test_all.dir/build.make core/tests/CMakeFiles/core_test_all.dir/test_config.o.provides.build
.PHONY : core/tests/CMakeFiles/core_test_all.dir/test_config.o.provides

core/tests/CMakeFiles/core_test_all.dir/test_config.o.provides.build: core/tests/CMakeFiles/core_test_all.dir/test_config.o
.PHONY : core/tests/CMakeFiles/core_test_all.dir/test_config.o.provides.build

core/tests/CMakeFiles/core_test_all.dir/test_build_info.o: core/tests/CMakeFiles/core_test_all.dir/flags.make
core/tests/CMakeFiles/core_test_all.dir/test_build_info.o: /home/antiw/demos/vxl-1.13.0/core/tests/test_build_info.cxx
	$(CMAKE_COMMAND) -E cmake_progress_report "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/CMakeFiles" $(CMAKE_PROGRESS_3)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object core/tests/CMakeFiles/core_test_all.dir/test_build_info.o"
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/core/tests" && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/core_test_all.dir/test_build_info.o -c /home/antiw/demos/vxl-1.13.0/core/tests/test_build_info.cxx

core/tests/CMakeFiles/core_test_all.dir/test_build_info.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/core_test_all.dir/test_build_info.i"
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/core/tests" && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/antiw/demos/vxl-1.13.0/core/tests/test_build_info.cxx > CMakeFiles/core_test_all.dir/test_build_info.i

core/tests/CMakeFiles/core_test_all.dir/test_build_info.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/core_test_all.dir/test_build_info.s"
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/core/tests" && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/antiw/demos/vxl-1.13.0/core/tests/test_build_info.cxx -o CMakeFiles/core_test_all.dir/test_build_info.s

core/tests/CMakeFiles/core_test_all.dir/test_build_info.o.requires:
.PHONY : core/tests/CMakeFiles/core_test_all.dir/test_build_info.o.requires

core/tests/CMakeFiles/core_test_all.dir/test_build_info.o.provides: core/tests/CMakeFiles/core_test_all.dir/test_build_info.o.requires
	$(MAKE) -f core/tests/CMakeFiles/core_test_all.dir/build.make core/tests/CMakeFiles/core_test_all.dir/test_build_info.o.provides.build
.PHONY : core/tests/CMakeFiles/core_test_all.dir/test_build_info.o.provides

core/tests/CMakeFiles/core_test_all.dir/test_build_info.o.provides.build: core/tests/CMakeFiles/core_test_all.dir/test_build_info.o
.PHONY : core/tests/CMakeFiles/core_test_all.dir/test_build_info.o.provides.build

# Object files for target core_test_all
core_test_all_OBJECTS = \
"CMakeFiles/core_test_all.dir/test_driver.o" \
"CMakeFiles/core_test_all.dir/test_config.o" \
"CMakeFiles/core_test_all.dir/test_build_info.o"

# External object files for target core_test_all
core_test_all_EXTERNAL_OBJECTS =

core/tests/core_test_all: core/tests/CMakeFiles/core_test_all.dir/test_driver.o
core/tests/core_test_all: core/tests/CMakeFiles/core_test_all.dir/test_config.o
core/tests/core_test_all: core/tests/CMakeFiles/core_test_all.dir/test_build_info.o
core/tests/core_test_all: lib/libtestlib.a
core/tests/core_test_all: lib/libvcl.a
core/tests/core_test_all: core/tests/CMakeFiles/core_test_all.dir/build.make
core/tests/core_test_all: core/tests/CMakeFiles/core_test_all.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable core_test_all"
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/core/tests" && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/core_test_all.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
core/tests/CMakeFiles/core_test_all.dir/build: core/tests/core_test_all
.PHONY : core/tests/CMakeFiles/core_test_all.dir/build

core/tests/CMakeFiles/core_test_all.dir/requires: core/tests/CMakeFiles/core_test_all.dir/test_driver.o.requires
core/tests/CMakeFiles/core_test_all.dir/requires: core/tests/CMakeFiles/core_test_all.dir/test_config.o.requires
core/tests/CMakeFiles/core_test_all.dir/requires: core/tests/CMakeFiles/core_test_all.dir/test_build_info.o.requires
.PHONY : core/tests/CMakeFiles/core_test_all.dir/requires

core/tests/CMakeFiles/core_test_all.dir/clean:
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/core/tests" && $(CMAKE_COMMAND) -P CMakeFiles/core_test_all.dir/cmake_clean.cmake
.PHONY : core/tests/CMakeFiles/core_test_all.dir/clean

core/tests/CMakeFiles/core_test_all.dir/depend:
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/antiw/demos/vxl-1.13.0 /home/antiw/demos/vxl-1.13.0/core/tests "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin" "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/core/tests" "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/core/tests/CMakeFiles/core_test_all.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : core/tests/CMakeFiles/core_test_all.dir/depend

