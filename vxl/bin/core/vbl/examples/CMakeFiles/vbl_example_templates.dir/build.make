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
include core/vbl/examples/CMakeFiles/vbl_example_templates.dir/depend.make

# Include the progress variables for this target.
include core/vbl/examples/CMakeFiles/vbl_example_templates.dir/progress.make

# Include the compile flags for this target's objects.
include core/vbl/examples/CMakeFiles/vbl_example_templates.dir/flags.make

core/vbl/examples/CMakeFiles/vbl_example_templates.dir/Templates/smartptr-instances.o: core/vbl/examples/CMakeFiles/vbl_example_templates.dir/flags.make
core/vbl/examples/CMakeFiles/vbl_example_templates.dir/Templates/smartptr-instances.o: /home/antiw/demos/vxl-1.13.0/core/vbl/examples/Templates/smartptr-instances.cxx
	$(CMAKE_COMMAND) -E cmake_progress_report "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/CMakeFiles" $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object core/vbl/examples/CMakeFiles/vbl_example_templates.dir/Templates/smartptr-instances.o"
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/core/vbl/examples" && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/vbl_example_templates.dir/Templates/smartptr-instances.o -c /home/antiw/demos/vxl-1.13.0/core/vbl/examples/Templates/smartptr-instances.cxx

core/vbl/examples/CMakeFiles/vbl_example_templates.dir/Templates/smartptr-instances.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/vbl_example_templates.dir/Templates/smartptr-instances.i"
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/core/vbl/examples" && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/antiw/demos/vxl-1.13.0/core/vbl/examples/Templates/smartptr-instances.cxx > CMakeFiles/vbl_example_templates.dir/Templates/smartptr-instances.i

core/vbl/examples/CMakeFiles/vbl_example_templates.dir/Templates/smartptr-instances.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/vbl_example_templates.dir/Templates/smartptr-instances.s"
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/core/vbl/examples" && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/antiw/demos/vxl-1.13.0/core/vbl/examples/Templates/smartptr-instances.cxx -o CMakeFiles/vbl_example_templates.dir/Templates/smartptr-instances.s

core/vbl/examples/CMakeFiles/vbl_example_templates.dir/Templates/smartptr-instances.o.requires:
.PHONY : core/vbl/examples/CMakeFiles/vbl_example_templates.dir/Templates/smartptr-instances.o.requires

core/vbl/examples/CMakeFiles/vbl_example_templates.dir/Templates/smartptr-instances.o.provides: core/vbl/examples/CMakeFiles/vbl_example_templates.dir/Templates/smartptr-instances.o.requires
	$(MAKE) -f core/vbl/examples/CMakeFiles/vbl_example_templates.dir/build.make core/vbl/examples/CMakeFiles/vbl_example_templates.dir/Templates/smartptr-instances.o.provides.build
.PHONY : core/vbl/examples/CMakeFiles/vbl_example_templates.dir/Templates/smartptr-instances.o.provides

core/vbl/examples/CMakeFiles/vbl_example_templates.dir/Templates/smartptr-instances.o.provides.build: core/vbl/examples/CMakeFiles/vbl_example_templates.dir/Templates/smartptr-instances.o
.PHONY : core/vbl/examples/CMakeFiles/vbl_example_templates.dir/Templates/smartptr-instances.o.provides.build

# Object files for target vbl_example_templates
vbl_example_templates_OBJECTS = \
"CMakeFiles/vbl_example_templates.dir/Templates/smartptr-instances.o"

# External object files for target vbl_example_templates
vbl_example_templates_EXTERNAL_OBJECTS =

lib/libvbl_example_templates.a: core/vbl/examples/CMakeFiles/vbl_example_templates.dir/Templates/smartptr-instances.o
lib/libvbl_example_templates.a: core/vbl/examples/CMakeFiles/vbl_example_templates.dir/build.make
lib/libvbl_example_templates.a: core/vbl/examples/CMakeFiles/vbl_example_templates.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX static library ../../../lib/libvbl_example_templates.a"
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/core/vbl/examples" && $(CMAKE_COMMAND) -P CMakeFiles/vbl_example_templates.dir/cmake_clean_target.cmake
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/core/vbl/examples" && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/vbl_example_templates.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
core/vbl/examples/CMakeFiles/vbl_example_templates.dir/build: lib/libvbl_example_templates.a
.PHONY : core/vbl/examples/CMakeFiles/vbl_example_templates.dir/build

core/vbl/examples/CMakeFiles/vbl_example_templates.dir/requires: core/vbl/examples/CMakeFiles/vbl_example_templates.dir/Templates/smartptr-instances.o.requires
.PHONY : core/vbl/examples/CMakeFiles/vbl_example_templates.dir/requires

core/vbl/examples/CMakeFiles/vbl_example_templates.dir/clean:
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/core/vbl/examples" && $(CMAKE_COMMAND) -P CMakeFiles/vbl_example_templates.dir/cmake_clean.cmake
.PHONY : core/vbl/examples/CMakeFiles/vbl_example_templates.dir/clean

core/vbl/examples/CMakeFiles/vbl_example_templates.dir/depend:
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/antiw/demos/vxl-1.13.0 /home/antiw/demos/vxl-1.13.0/core/vbl/examples "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin" "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/core/vbl/examples" "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/core/vbl/examples/CMakeFiles/vbl_example_templates.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : core/vbl/examples/CMakeFiles/vbl_example_templates.dir/depend

