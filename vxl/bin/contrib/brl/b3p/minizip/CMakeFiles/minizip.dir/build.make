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
include contrib/brl/b3p/minizip/CMakeFiles/minizip.dir/depend.make

# Include the progress variables for this target.
include contrib/brl/b3p/minizip/CMakeFiles/minizip.dir/progress.make

# Include the compile flags for this target's objects.
include contrib/brl/b3p/minizip/CMakeFiles/minizip.dir/flags.make

contrib/brl/b3p/minizip/CMakeFiles/minizip.dir/ioapi.o: contrib/brl/b3p/minizip/CMakeFiles/minizip.dir/flags.make
contrib/brl/b3p/minizip/CMakeFiles/minizip.dir/ioapi.o: /home/antiw/demos/vxl-1.13.0/contrib/brl/b3p/minizip/ioapi.c
	$(CMAKE_COMMAND) -E cmake_progress_report "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/CMakeFiles" $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object contrib/brl/b3p/minizip/CMakeFiles/minizip.dir/ioapi.o"
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/contrib/brl/b3p/minizip" && /usr/bin/gcc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/minizip.dir/ioapi.o   -c /home/antiw/demos/vxl-1.13.0/contrib/brl/b3p/minizip/ioapi.c

contrib/brl/b3p/minizip/CMakeFiles/minizip.dir/ioapi.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/minizip.dir/ioapi.i"
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/contrib/brl/b3p/minizip" && /usr/bin/gcc  $(C_DEFINES) $(C_FLAGS) -E /home/antiw/demos/vxl-1.13.0/contrib/brl/b3p/minizip/ioapi.c > CMakeFiles/minizip.dir/ioapi.i

contrib/brl/b3p/minizip/CMakeFiles/minizip.dir/ioapi.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/minizip.dir/ioapi.s"
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/contrib/brl/b3p/minizip" && /usr/bin/gcc  $(C_DEFINES) $(C_FLAGS) -S /home/antiw/demos/vxl-1.13.0/contrib/brl/b3p/minizip/ioapi.c -o CMakeFiles/minizip.dir/ioapi.s

contrib/brl/b3p/minizip/CMakeFiles/minizip.dir/ioapi.o.requires:
.PHONY : contrib/brl/b3p/minizip/CMakeFiles/minizip.dir/ioapi.o.requires

contrib/brl/b3p/minizip/CMakeFiles/minizip.dir/ioapi.o.provides: contrib/brl/b3p/minizip/CMakeFiles/minizip.dir/ioapi.o.requires
	$(MAKE) -f contrib/brl/b3p/minizip/CMakeFiles/minizip.dir/build.make contrib/brl/b3p/minizip/CMakeFiles/minizip.dir/ioapi.o.provides.build
.PHONY : contrib/brl/b3p/minizip/CMakeFiles/minizip.dir/ioapi.o.provides

contrib/brl/b3p/minizip/CMakeFiles/minizip.dir/ioapi.o.provides.build: contrib/brl/b3p/minizip/CMakeFiles/minizip.dir/ioapi.o
.PHONY : contrib/brl/b3p/minizip/CMakeFiles/minizip.dir/ioapi.o.provides.build

contrib/brl/b3p/minizip/CMakeFiles/minizip.dir/zip.o: contrib/brl/b3p/minizip/CMakeFiles/minizip.dir/flags.make
contrib/brl/b3p/minizip/CMakeFiles/minizip.dir/zip.o: /home/antiw/demos/vxl-1.13.0/contrib/brl/b3p/minizip/zip.c
	$(CMAKE_COMMAND) -E cmake_progress_report "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/CMakeFiles" $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object contrib/brl/b3p/minizip/CMakeFiles/minizip.dir/zip.o"
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/contrib/brl/b3p/minizip" && /usr/bin/gcc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/minizip.dir/zip.o   -c /home/antiw/demos/vxl-1.13.0/contrib/brl/b3p/minizip/zip.c

contrib/brl/b3p/minizip/CMakeFiles/minizip.dir/zip.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/minizip.dir/zip.i"
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/contrib/brl/b3p/minizip" && /usr/bin/gcc  $(C_DEFINES) $(C_FLAGS) -E /home/antiw/demos/vxl-1.13.0/contrib/brl/b3p/minizip/zip.c > CMakeFiles/minizip.dir/zip.i

contrib/brl/b3p/minizip/CMakeFiles/minizip.dir/zip.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/minizip.dir/zip.s"
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/contrib/brl/b3p/minizip" && /usr/bin/gcc  $(C_DEFINES) $(C_FLAGS) -S /home/antiw/demos/vxl-1.13.0/contrib/brl/b3p/minizip/zip.c -o CMakeFiles/minizip.dir/zip.s

contrib/brl/b3p/minizip/CMakeFiles/minizip.dir/zip.o.requires:
.PHONY : contrib/brl/b3p/minizip/CMakeFiles/minizip.dir/zip.o.requires

contrib/brl/b3p/minizip/CMakeFiles/minizip.dir/zip.o.provides: contrib/brl/b3p/minizip/CMakeFiles/minizip.dir/zip.o.requires
	$(MAKE) -f contrib/brl/b3p/minizip/CMakeFiles/minizip.dir/build.make contrib/brl/b3p/minizip/CMakeFiles/minizip.dir/zip.o.provides.build
.PHONY : contrib/brl/b3p/minizip/CMakeFiles/minizip.dir/zip.o.provides

contrib/brl/b3p/minizip/CMakeFiles/minizip.dir/zip.o.provides.build: contrib/brl/b3p/minizip/CMakeFiles/minizip.dir/zip.o
.PHONY : contrib/brl/b3p/minizip/CMakeFiles/minizip.dir/zip.o.provides.build

contrib/brl/b3p/minizip/CMakeFiles/minizip.dir/unzip.o: contrib/brl/b3p/minizip/CMakeFiles/minizip.dir/flags.make
contrib/brl/b3p/minizip/CMakeFiles/minizip.dir/unzip.o: /home/antiw/demos/vxl-1.13.0/contrib/brl/b3p/minizip/unzip.c
	$(CMAKE_COMMAND) -E cmake_progress_report "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/CMakeFiles" $(CMAKE_PROGRESS_3)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object contrib/brl/b3p/minizip/CMakeFiles/minizip.dir/unzip.o"
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/contrib/brl/b3p/minizip" && /usr/bin/gcc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/minizip.dir/unzip.o   -c /home/antiw/demos/vxl-1.13.0/contrib/brl/b3p/minizip/unzip.c

contrib/brl/b3p/minizip/CMakeFiles/minizip.dir/unzip.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/minizip.dir/unzip.i"
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/contrib/brl/b3p/minizip" && /usr/bin/gcc  $(C_DEFINES) $(C_FLAGS) -E /home/antiw/demos/vxl-1.13.0/contrib/brl/b3p/minizip/unzip.c > CMakeFiles/minizip.dir/unzip.i

contrib/brl/b3p/minizip/CMakeFiles/minizip.dir/unzip.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/minizip.dir/unzip.s"
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/contrib/brl/b3p/minizip" && /usr/bin/gcc  $(C_DEFINES) $(C_FLAGS) -S /home/antiw/demos/vxl-1.13.0/contrib/brl/b3p/minizip/unzip.c -o CMakeFiles/minizip.dir/unzip.s

contrib/brl/b3p/minizip/CMakeFiles/minizip.dir/unzip.o.requires:
.PHONY : contrib/brl/b3p/minizip/CMakeFiles/minizip.dir/unzip.o.requires

contrib/brl/b3p/minizip/CMakeFiles/minizip.dir/unzip.o.provides: contrib/brl/b3p/minizip/CMakeFiles/minizip.dir/unzip.o.requires
	$(MAKE) -f contrib/brl/b3p/minizip/CMakeFiles/minizip.dir/build.make contrib/brl/b3p/minizip/CMakeFiles/minizip.dir/unzip.o.provides.build
.PHONY : contrib/brl/b3p/minizip/CMakeFiles/minizip.dir/unzip.o.provides

contrib/brl/b3p/minizip/CMakeFiles/minizip.dir/unzip.o.provides.build: contrib/brl/b3p/minizip/CMakeFiles/minizip.dir/unzip.o
.PHONY : contrib/brl/b3p/minizip/CMakeFiles/minizip.dir/unzip.o.provides.build

contrib/brl/b3p/minizip/CMakeFiles/minizip.dir/mztools.o: contrib/brl/b3p/minizip/CMakeFiles/minizip.dir/flags.make
contrib/brl/b3p/minizip/CMakeFiles/minizip.dir/mztools.o: /home/antiw/demos/vxl-1.13.0/contrib/brl/b3p/minizip/mztools.c
	$(CMAKE_COMMAND) -E cmake_progress_report "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/CMakeFiles" $(CMAKE_PROGRESS_4)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object contrib/brl/b3p/minizip/CMakeFiles/minizip.dir/mztools.o"
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/contrib/brl/b3p/minizip" && /usr/bin/gcc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/minizip.dir/mztools.o   -c /home/antiw/demos/vxl-1.13.0/contrib/brl/b3p/minizip/mztools.c

contrib/brl/b3p/minizip/CMakeFiles/minizip.dir/mztools.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/minizip.dir/mztools.i"
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/contrib/brl/b3p/minizip" && /usr/bin/gcc  $(C_DEFINES) $(C_FLAGS) -E /home/antiw/demos/vxl-1.13.0/contrib/brl/b3p/minizip/mztools.c > CMakeFiles/minizip.dir/mztools.i

contrib/brl/b3p/minizip/CMakeFiles/minizip.dir/mztools.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/minizip.dir/mztools.s"
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/contrib/brl/b3p/minizip" && /usr/bin/gcc  $(C_DEFINES) $(C_FLAGS) -S /home/antiw/demos/vxl-1.13.0/contrib/brl/b3p/minizip/mztools.c -o CMakeFiles/minizip.dir/mztools.s

contrib/brl/b3p/minizip/CMakeFiles/minizip.dir/mztools.o.requires:
.PHONY : contrib/brl/b3p/minizip/CMakeFiles/minizip.dir/mztools.o.requires

contrib/brl/b3p/minizip/CMakeFiles/minizip.dir/mztools.o.provides: contrib/brl/b3p/minizip/CMakeFiles/minizip.dir/mztools.o.requires
	$(MAKE) -f contrib/brl/b3p/minizip/CMakeFiles/minizip.dir/build.make contrib/brl/b3p/minizip/CMakeFiles/minizip.dir/mztools.o.provides.build
.PHONY : contrib/brl/b3p/minizip/CMakeFiles/minizip.dir/mztools.o.provides

contrib/brl/b3p/minizip/CMakeFiles/minizip.dir/mztools.o.provides.build: contrib/brl/b3p/minizip/CMakeFiles/minizip.dir/mztools.o
.PHONY : contrib/brl/b3p/minizip/CMakeFiles/minizip.dir/mztools.o.provides.build

# Object files for target minizip
minizip_OBJECTS = \
"CMakeFiles/minizip.dir/ioapi.o" \
"CMakeFiles/minizip.dir/zip.o" \
"CMakeFiles/minizip.dir/unzip.o" \
"CMakeFiles/minizip.dir/mztools.o"

# External object files for target minizip
minizip_EXTERNAL_OBJECTS =

lib/libminizip.a: contrib/brl/b3p/minizip/CMakeFiles/minizip.dir/ioapi.o
lib/libminizip.a: contrib/brl/b3p/minizip/CMakeFiles/minizip.dir/zip.o
lib/libminizip.a: contrib/brl/b3p/minizip/CMakeFiles/minizip.dir/unzip.o
lib/libminizip.a: contrib/brl/b3p/minizip/CMakeFiles/minizip.dir/mztools.o
lib/libminizip.a: contrib/brl/b3p/minizip/CMakeFiles/minizip.dir/build.make
lib/libminizip.a: contrib/brl/b3p/minizip/CMakeFiles/minizip.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking C static library ../../../../lib/libminizip.a"
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/contrib/brl/b3p/minizip" && $(CMAKE_COMMAND) -P CMakeFiles/minizip.dir/cmake_clean_target.cmake
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/contrib/brl/b3p/minizip" && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/minizip.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
contrib/brl/b3p/minizip/CMakeFiles/minizip.dir/build: lib/libminizip.a
.PHONY : contrib/brl/b3p/minizip/CMakeFiles/minizip.dir/build

contrib/brl/b3p/minizip/CMakeFiles/minizip.dir/requires: contrib/brl/b3p/minizip/CMakeFiles/minizip.dir/ioapi.o.requires
contrib/brl/b3p/minizip/CMakeFiles/minizip.dir/requires: contrib/brl/b3p/minizip/CMakeFiles/minizip.dir/zip.o.requires
contrib/brl/b3p/minizip/CMakeFiles/minizip.dir/requires: contrib/brl/b3p/minizip/CMakeFiles/minizip.dir/unzip.o.requires
contrib/brl/b3p/minizip/CMakeFiles/minizip.dir/requires: contrib/brl/b3p/minizip/CMakeFiles/minizip.dir/mztools.o.requires
.PHONY : contrib/brl/b3p/minizip/CMakeFiles/minizip.dir/requires

contrib/brl/b3p/minizip/CMakeFiles/minizip.dir/clean:
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/contrib/brl/b3p/minizip" && $(CMAKE_COMMAND) -P CMakeFiles/minizip.dir/cmake_clean.cmake
.PHONY : contrib/brl/b3p/minizip/CMakeFiles/minizip.dir/clean

contrib/brl/b3p/minizip/CMakeFiles/minizip.dir/depend:
	cd "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/antiw/demos/vxl-1.13.0 /home/antiw/demos/vxl-1.13.0/contrib/brl/b3p/minizip "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin" "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/contrib/brl/b3p/minizip" "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/contrib/brl/b3p/minizip/CMakeFiles/minizip.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : contrib/brl/b3p/minizip/CMakeFiles/minizip.dir/depend

