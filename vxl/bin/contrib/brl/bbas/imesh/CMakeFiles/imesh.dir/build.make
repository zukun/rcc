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
include contrib/brl/bbas/imesh/CMakeFiles/imesh.dir/depend.make

# Include the progress variables for this target.
include contrib/brl/bbas/imesh/CMakeFiles/imesh.dir/progress.make

# Include the compile flags for this target's objects.
include contrib/brl/bbas/imesh/CMakeFiles/imesh.dir/flags.make

contrib/brl/bbas/imesh/CMakeFiles/imesh.dir/imesh_vertex.o: contrib/brl/bbas/imesh/CMakeFiles/imesh.dir/flags.make
contrib/brl/bbas/imesh/CMakeFiles/imesh.dir/imesh_vertex.o: /home/antiw/demos/vxl-1.13.0/contrib/brl/bbas/imesh/imesh_vertex.cxx
	$(CMAKE_COMMAND) -E cmake_progress_report "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/CMakeFiles" $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object contrib/brl/bbas/imesh/CMakeFiles/imesh.dir/imesh_vertex.o"
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/contrib/brl/bbas/imesh" && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/imesh.dir/imesh_vertex.o -c /home/antiw/demos/vxl-1.13.0/contrib/brl/bbas/imesh/imesh_vertex.cxx

contrib/brl/bbas/imesh/CMakeFiles/imesh.dir/imesh_vertex.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/imesh.dir/imesh_vertex.i"
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/contrib/brl/bbas/imesh" && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/antiw/demos/vxl-1.13.0/contrib/brl/bbas/imesh/imesh_vertex.cxx > CMakeFiles/imesh.dir/imesh_vertex.i

contrib/brl/bbas/imesh/CMakeFiles/imesh.dir/imesh_vertex.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/imesh.dir/imesh_vertex.s"
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/contrib/brl/bbas/imesh" && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/antiw/demos/vxl-1.13.0/contrib/brl/bbas/imesh/imesh_vertex.cxx -o CMakeFiles/imesh.dir/imesh_vertex.s

contrib/brl/bbas/imesh/CMakeFiles/imesh.dir/imesh_vertex.o.requires:
.PHONY : contrib/brl/bbas/imesh/CMakeFiles/imesh.dir/imesh_vertex.o.requires

contrib/brl/bbas/imesh/CMakeFiles/imesh.dir/imesh_vertex.o.provides: contrib/brl/bbas/imesh/CMakeFiles/imesh.dir/imesh_vertex.o.requires
	$(MAKE) -f contrib/brl/bbas/imesh/CMakeFiles/imesh.dir/build.make contrib/brl/bbas/imesh/CMakeFiles/imesh.dir/imesh_vertex.o.provides.build
.PHONY : contrib/brl/bbas/imesh/CMakeFiles/imesh.dir/imesh_vertex.o.provides

contrib/brl/bbas/imesh/CMakeFiles/imesh.dir/imesh_vertex.o.provides.build: contrib/brl/bbas/imesh/CMakeFiles/imesh.dir/imesh_vertex.o
.PHONY : contrib/brl/bbas/imesh/CMakeFiles/imesh.dir/imesh_vertex.o.provides.build

contrib/brl/bbas/imesh/CMakeFiles/imesh.dir/imesh_face.o: contrib/brl/bbas/imesh/CMakeFiles/imesh.dir/flags.make
contrib/brl/bbas/imesh/CMakeFiles/imesh.dir/imesh_face.o: /home/antiw/demos/vxl-1.13.0/contrib/brl/bbas/imesh/imesh_face.cxx
	$(CMAKE_COMMAND) -E cmake_progress_report "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/CMakeFiles" $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object contrib/brl/bbas/imesh/CMakeFiles/imesh.dir/imesh_face.o"
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/contrib/brl/bbas/imesh" && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/imesh.dir/imesh_face.o -c /home/antiw/demos/vxl-1.13.0/contrib/brl/bbas/imesh/imesh_face.cxx

contrib/brl/bbas/imesh/CMakeFiles/imesh.dir/imesh_face.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/imesh.dir/imesh_face.i"
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/contrib/brl/bbas/imesh" && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/antiw/demos/vxl-1.13.0/contrib/brl/bbas/imesh/imesh_face.cxx > CMakeFiles/imesh.dir/imesh_face.i

contrib/brl/bbas/imesh/CMakeFiles/imesh.dir/imesh_face.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/imesh.dir/imesh_face.s"
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/contrib/brl/bbas/imesh" && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/antiw/demos/vxl-1.13.0/contrib/brl/bbas/imesh/imesh_face.cxx -o CMakeFiles/imesh.dir/imesh_face.s

contrib/brl/bbas/imesh/CMakeFiles/imesh.dir/imesh_face.o.requires:
.PHONY : contrib/brl/bbas/imesh/CMakeFiles/imesh.dir/imesh_face.o.requires

contrib/brl/bbas/imesh/CMakeFiles/imesh.dir/imesh_face.o.provides: contrib/brl/bbas/imesh/CMakeFiles/imesh.dir/imesh_face.o.requires
	$(MAKE) -f contrib/brl/bbas/imesh/CMakeFiles/imesh.dir/build.make contrib/brl/bbas/imesh/CMakeFiles/imesh.dir/imesh_face.o.provides.build
.PHONY : contrib/brl/bbas/imesh/CMakeFiles/imesh.dir/imesh_face.o.provides

contrib/brl/bbas/imesh/CMakeFiles/imesh.dir/imesh_face.o.provides.build: contrib/brl/bbas/imesh/CMakeFiles/imesh.dir/imesh_face.o
.PHONY : contrib/brl/bbas/imesh/CMakeFiles/imesh.dir/imesh_face.o.provides.build

contrib/brl/bbas/imesh/CMakeFiles/imesh.dir/imesh_half_edge.o: contrib/brl/bbas/imesh/CMakeFiles/imesh.dir/flags.make
contrib/brl/bbas/imesh/CMakeFiles/imesh.dir/imesh_half_edge.o: /home/antiw/demos/vxl-1.13.0/contrib/brl/bbas/imesh/imesh_half_edge.cxx
	$(CMAKE_COMMAND) -E cmake_progress_report "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/CMakeFiles" $(CMAKE_PROGRESS_3)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object contrib/brl/bbas/imesh/CMakeFiles/imesh.dir/imesh_half_edge.o"
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/contrib/brl/bbas/imesh" && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/imesh.dir/imesh_half_edge.o -c /home/antiw/demos/vxl-1.13.0/contrib/brl/bbas/imesh/imesh_half_edge.cxx

contrib/brl/bbas/imesh/CMakeFiles/imesh.dir/imesh_half_edge.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/imesh.dir/imesh_half_edge.i"
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/contrib/brl/bbas/imesh" && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/antiw/demos/vxl-1.13.0/contrib/brl/bbas/imesh/imesh_half_edge.cxx > CMakeFiles/imesh.dir/imesh_half_edge.i

contrib/brl/bbas/imesh/CMakeFiles/imesh.dir/imesh_half_edge.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/imesh.dir/imesh_half_edge.s"
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/contrib/brl/bbas/imesh" && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/antiw/demos/vxl-1.13.0/contrib/brl/bbas/imesh/imesh_half_edge.cxx -o CMakeFiles/imesh.dir/imesh_half_edge.s

contrib/brl/bbas/imesh/CMakeFiles/imesh.dir/imesh_half_edge.o.requires:
.PHONY : contrib/brl/bbas/imesh/CMakeFiles/imesh.dir/imesh_half_edge.o.requires

contrib/brl/bbas/imesh/CMakeFiles/imesh.dir/imesh_half_edge.o.provides: contrib/brl/bbas/imesh/CMakeFiles/imesh.dir/imesh_half_edge.o.requires
	$(MAKE) -f contrib/brl/bbas/imesh/CMakeFiles/imesh.dir/build.make contrib/brl/bbas/imesh/CMakeFiles/imesh.dir/imesh_half_edge.o.provides.build
.PHONY : contrib/brl/bbas/imesh/CMakeFiles/imesh.dir/imesh_half_edge.o.provides

contrib/brl/bbas/imesh/CMakeFiles/imesh.dir/imesh_half_edge.o.provides.build: contrib/brl/bbas/imesh/CMakeFiles/imesh.dir/imesh_half_edge.o
.PHONY : contrib/brl/bbas/imesh/CMakeFiles/imesh.dir/imesh_half_edge.o.provides.build

contrib/brl/bbas/imesh/CMakeFiles/imesh.dir/imesh_mesh.o: contrib/brl/bbas/imesh/CMakeFiles/imesh.dir/flags.make
contrib/brl/bbas/imesh/CMakeFiles/imesh.dir/imesh_mesh.o: /home/antiw/demos/vxl-1.13.0/contrib/brl/bbas/imesh/imesh_mesh.cxx
	$(CMAKE_COMMAND) -E cmake_progress_report "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/CMakeFiles" $(CMAKE_PROGRESS_4)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object contrib/brl/bbas/imesh/CMakeFiles/imesh.dir/imesh_mesh.o"
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/contrib/brl/bbas/imesh" && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/imesh.dir/imesh_mesh.o -c /home/antiw/demos/vxl-1.13.0/contrib/brl/bbas/imesh/imesh_mesh.cxx

contrib/brl/bbas/imesh/CMakeFiles/imesh.dir/imesh_mesh.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/imesh.dir/imesh_mesh.i"
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/contrib/brl/bbas/imesh" && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/antiw/demos/vxl-1.13.0/contrib/brl/bbas/imesh/imesh_mesh.cxx > CMakeFiles/imesh.dir/imesh_mesh.i

contrib/brl/bbas/imesh/CMakeFiles/imesh.dir/imesh_mesh.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/imesh.dir/imesh_mesh.s"
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/contrib/brl/bbas/imesh" && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/antiw/demos/vxl-1.13.0/contrib/brl/bbas/imesh/imesh_mesh.cxx -o CMakeFiles/imesh.dir/imesh_mesh.s

contrib/brl/bbas/imesh/CMakeFiles/imesh.dir/imesh_mesh.o.requires:
.PHONY : contrib/brl/bbas/imesh/CMakeFiles/imesh.dir/imesh_mesh.o.requires

contrib/brl/bbas/imesh/CMakeFiles/imesh.dir/imesh_mesh.o.provides: contrib/brl/bbas/imesh/CMakeFiles/imesh.dir/imesh_mesh.o.requires
	$(MAKE) -f contrib/brl/bbas/imesh/CMakeFiles/imesh.dir/build.make contrib/brl/bbas/imesh/CMakeFiles/imesh.dir/imesh_mesh.o.provides.build
.PHONY : contrib/brl/bbas/imesh/CMakeFiles/imesh.dir/imesh_mesh.o.provides

contrib/brl/bbas/imesh/CMakeFiles/imesh.dir/imesh_mesh.o.provides.build: contrib/brl/bbas/imesh/CMakeFiles/imesh.dir/imesh_mesh.o
.PHONY : contrib/brl/bbas/imesh/CMakeFiles/imesh.dir/imesh_mesh.o.provides.build

contrib/brl/bbas/imesh/CMakeFiles/imesh.dir/imesh_fileio.o: contrib/brl/bbas/imesh/CMakeFiles/imesh.dir/flags.make
contrib/brl/bbas/imesh/CMakeFiles/imesh.dir/imesh_fileio.o: /home/antiw/demos/vxl-1.13.0/contrib/brl/bbas/imesh/imesh_fileio.cxx
	$(CMAKE_COMMAND) -E cmake_progress_report "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/CMakeFiles" $(CMAKE_PROGRESS_5)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object contrib/brl/bbas/imesh/CMakeFiles/imesh.dir/imesh_fileio.o"
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/contrib/brl/bbas/imesh" && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/imesh.dir/imesh_fileio.o -c /home/antiw/demos/vxl-1.13.0/contrib/brl/bbas/imesh/imesh_fileio.cxx

contrib/brl/bbas/imesh/CMakeFiles/imesh.dir/imesh_fileio.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/imesh.dir/imesh_fileio.i"
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/contrib/brl/bbas/imesh" && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/antiw/demos/vxl-1.13.0/contrib/brl/bbas/imesh/imesh_fileio.cxx > CMakeFiles/imesh.dir/imesh_fileio.i

contrib/brl/bbas/imesh/CMakeFiles/imesh.dir/imesh_fileio.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/imesh.dir/imesh_fileio.s"
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/contrib/brl/bbas/imesh" && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/antiw/demos/vxl-1.13.0/contrib/brl/bbas/imesh/imesh_fileio.cxx -o CMakeFiles/imesh.dir/imesh_fileio.s

contrib/brl/bbas/imesh/CMakeFiles/imesh.dir/imesh_fileio.o.requires:
.PHONY : contrib/brl/bbas/imesh/CMakeFiles/imesh.dir/imesh_fileio.o.requires

contrib/brl/bbas/imesh/CMakeFiles/imesh.dir/imesh_fileio.o.provides: contrib/brl/bbas/imesh/CMakeFiles/imesh.dir/imesh_fileio.o.requires
	$(MAKE) -f contrib/brl/bbas/imesh/CMakeFiles/imesh.dir/build.make contrib/brl/bbas/imesh/CMakeFiles/imesh.dir/imesh_fileio.o.provides.build
.PHONY : contrib/brl/bbas/imesh/CMakeFiles/imesh.dir/imesh_fileio.o.provides

contrib/brl/bbas/imesh/CMakeFiles/imesh.dir/imesh_fileio.o.provides.build: contrib/brl/bbas/imesh/CMakeFiles/imesh.dir/imesh_fileio.o
.PHONY : contrib/brl/bbas/imesh/CMakeFiles/imesh.dir/imesh_fileio.o.provides.build

contrib/brl/bbas/imesh/CMakeFiles/imesh.dir/imesh_operations.o: contrib/brl/bbas/imesh/CMakeFiles/imesh.dir/flags.make
contrib/brl/bbas/imesh/CMakeFiles/imesh.dir/imesh_operations.o: /home/antiw/demos/vxl-1.13.0/contrib/brl/bbas/imesh/imesh_operations.cxx
	$(CMAKE_COMMAND) -E cmake_progress_report "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/CMakeFiles" $(CMAKE_PROGRESS_6)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object contrib/brl/bbas/imesh/CMakeFiles/imesh.dir/imesh_operations.o"
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/contrib/brl/bbas/imesh" && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/imesh.dir/imesh_operations.o -c /home/antiw/demos/vxl-1.13.0/contrib/brl/bbas/imesh/imesh_operations.cxx

contrib/brl/bbas/imesh/CMakeFiles/imesh.dir/imesh_operations.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/imesh.dir/imesh_operations.i"
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/contrib/brl/bbas/imesh" && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/antiw/demos/vxl-1.13.0/contrib/brl/bbas/imesh/imesh_operations.cxx > CMakeFiles/imesh.dir/imesh_operations.i

contrib/brl/bbas/imesh/CMakeFiles/imesh.dir/imesh_operations.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/imesh.dir/imesh_operations.s"
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/contrib/brl/bbas/imesh" && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/antiw/demos/vxl-1.13.0/contrib/brl/bbas/imesh/imesh_operations.cxx -o CMakeFiles/imesh.dir/imesh_operations.s

contrib/brl/bbas/imesh/CMakeFiles/imesh.dir/imesh_operations.o.requires:
.PHONY : contrib/brl/bbas/imesh/CMakeFiles/imesh.dir/imesh_operations.o.requires

contrib/brl/bbas/imesh/CMakeFiles/imesh.dir/imesh_operations.o.provides: contrib/brl/bbas/imesh/CMakeFiles/imesh.dir/imesh_operations.o.requires
	$(MAKE) -f contrib/brl/bbas/imesh/CMakeFiles/imesh.dir/build.make contrib/brl/bbas/imesh/CMakeFiles/imesh.dir/imesh_operations.o.provides.build
.PHONY : contrib/brl/bbas/imesh/CMakeFiles/imesh.dir/imesh_operations.o.provides

contrib/brl/bbas/imesh/CMakeFiles/imesh.dir/imesh_operations.o.provides.build: contrib/brl/bbas/imesh/CMakeFiles/imesh.dir/imesh_operations.o
.PHONY : contrib/brl/bbas/imesh/CMakeFiles/imesh.dir/imesh_operations.o.provides.build

contrib/brl/bbas/imesh/CMakeFiles/imesh.dir/imesh_detection.o: contrib/brl/bbas/imesh/CMakeFiles/imesh.dir/flags.make
contrib/brl/bbas/imesh/CMakeFiles/imesh.dir/imesh_detection.o: /home/antiw/demos/vxl-1.13.0/contrib/brl/bbas/imesh/imesh_detection.cxx
	$(CMAKE_COMMAND) -E cmake_progress_report "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/CMakeFiles" $(CMAKE_PROGRESS_7)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object contrib/brl/bbas/imesh/CMakeFiles/imesh.dir/imesh_detection.o"
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/contrib/brl/bbas/imesh" && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/imesh.dir/imesh_detection.o -c /home/antiw/demos/vxl-1.13.0/contrib/brl/bbas/imesh/imesh_detection.cxx

contrib/brl/bbas/imesh/CMakeFiles/imesh.dir/imesh_detection.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/imesh.dir/imesh_detection.i"
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/contrib/brl/bbas/imesh" && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/antiw/demos/vxl-1.13.0/contrib/brl/bbas/imesh/imesh_detection.cxx > CMakeFiles/imesh.dir/imesh_detection.i

contrib/brl/bbas/imesh/CMakeFiles/imesh.dir/imesh_detection.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/imesh.dir/imesh_detection.s"
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/contrib/brl/bbas/imesh" && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/antiw/demos/vxl-1.13.0/contrib/brl/bbas/imesh/imesh_detection.cxx -o CMakeFiles/imesh.dir/imesh_detection.s

contrib/brl/bbas/imesh/CMakeFiles/imesh.dir/imesh_detection.o.requires:
.PHONY : contrib/brl/bbas/imesh/CMakeFiles/imesh.dir/imesh_detection.o.requires

contrib/brl/bbas/imesh/CMakeFiles/imesh.dir/imesh_detection.o.provides: contrib/brl/bbas/imesh/CMakeFiles/imesh.dir/imesh_detection.o.requires
	$(MAKE) -f contrib/brl/bbas/imesh/CMakeFiles/imesh.dir/build.make contrib/brl/bbas/imesh/CMakeFiles/imesh.dir/imesh_detection.o.provides.build
.PHONY : contrib/brl/bbas/imesh/CMakeFiles/imesh.dir/imesh_detection.o.provides

contrib/brl/bbas/imesh/CMakeFiles/imesh.dir/imesh_detection.o.provides.build: contrib/brl/bbas/imesh/CMakeFiles/imesh.dir/imesh_detection.o
.PHONY : contrib/brl/bbas/imesh/CMakeFiles/imesh.dir/imesh_detection.o.provides.build

# Object files for target imesh
imesh_OBJECTS = \
"CMakeFiles/imesh.dir/imesh_vertex.o" \
"CMakeFiles/imesh.dir/imesh_face.o" \
"CMakeFiles/imesh.dir/imesh_half_edge.o" \
"CMakeFiles/imesh.dir/imesh_mesh.o" \
"CMakeFiles/imesh.dir/imesh_fileio.o" \
"CMakeFiles/imesh.dir/imesh_operations.o" \
"CMakeFiles/imesh.dir/imesh_detection.o"

# External object files for target imesh
imesh_EXTERNAL_OBJECTS =

lib/libimesh.a: contrib/brl/bbas/imesh/CMakeFiles/imesh.dir/imesh_vertex.o
lib/libimesh.a: contrib/brl/bbas/imesh/CMakeFiles/imesh.dir/imesh_face.o
lib/libimesh.a: contrib/brl/bbas/imesh/CMakeFiles/imesh.dir/imesh_half_edge.o
lib/libimesh.a: contrib/brl/bbas/imesh/CMakeFiles/imesh.dir/imesh_mesh.o
lib/libimesh.a: contrib/brl/bbas/imesh/CMakeFiles/imesh.dir/imesh_fileio.o
lib/libimesh.a: contrib/brl/bbas/imesh/CMakeFiles/imesh.dir/imesh_operations.o
lib/libimesh.a: contrib/brl/bbas/imesh/CMakeFiles/imesh.dir/imesh_detection.o
lib/libimesh.a: contrib/brl/bbas/imesh/CMakeFiles/imesh.dir/build.make
lib/libimesh.a: contrib/brl/bbas/imesh/CMakeFiles/imesh.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX static library ../../../../lib/libimesh.a"
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/contrib/brl/bbas/imesh" && $(CMAKE_COMMAND) -P CMakeFiles/imesh.dir/cmake_clean_target.cmake
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/contrib/brl/bbas/imesh" && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/imesh.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
contrib/brl/bbas/imesh/CMakeFiles/imesh.dir/build: lib/libimesh.a
.PHONY : contrib/brl/bbas/imesh/CMakeFiles/imesh.dir/build

contrib/brl/bbas/imesh/CMakeFiles/imesh.dir/requires: contrib/brl/bbas/imesh/CMakeFiles/imesh.dir/imesh_vertex.o.requires
contrib/brl/bbas/imesh/CMakeFiles/imesh.dir/requires: contrib/brl/bbas/imesh/CMakeFiles/imesh.dir/imesh_face.o.requires
contrib/brl/bbas/imesh/CMakeFiles/imesh.dir/requires: contrib/brl/bbas/imesh/CMakeFiles/imesh.dir/imesh_half_edge.o.requires
contrib/brl/bbas/imesh/CMakeFiles/imesh.dir/requires: contrib/brl/bbas/imesh/CMakeFiles/imesh.dir/imesh_mesh.o.requires
contrib/brl/bbas/imesh/CMakeFiles/imesh.dir/requires: contrib/brl/bbas/imesh/CMakeFiles/imesh.dir/imesh_fileio.o.requires
contrib/brl/bbas/imesh/CMakeFiles/imesh.dir/requires: contrib/brl/bbas/imesh/CMakeFiles/imesh.dir/imesh_operations.o.requires
contrib/brl/bbas/imesh/CMakeFiles/imesh.dir/requires: contrib/brl/bbas/imesh/CMakeFiles/imesh.dir/imesh_detection.o.requires
.PHONY : contrib/brl/bbas/imesh/CMakeFiles/imesh.dir/requires

contrib/brl/bbas/imesh/CMakeFiles/imesh.dir/clean:
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/contrib/brl/bbas/imesh" && $(CMAKE_COMMAND) -P CMakeFiles/imesh.dir/cmake_clean.cmake
.PHONY : contrib/brl/bbas/imesh/CMakeFiles/imesh.dir/clean

contrib/brl/bbas/imesh/CMakeFiles/imesh.dir/depend:
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/antiw/demos/vxl-1.13.0 /home/antiw/demos/vxl-1.13.0/contrib/brl/bbas/imesh "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin" "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/contrib/brl/bbas/imesh" "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/contrib/brl/bbas/imesh/CMakeFiles/imesh.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : contrib/brl/bbas/imesh/CMakeFiles/imesh.dir/depend

