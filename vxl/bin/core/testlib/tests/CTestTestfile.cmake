# CMake generated Testfile for 
# Source directory: /home/antiw/demos/vxl-1.13.0/core/testlib/tests
# Build directory: /mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/core/testlib/tests
# 
# This file replicates the SUBDIRS() and ADD_TEST() commands from the source
# tree CMakeLists.txt file, skipping any SUBDIRS() or ADD_TEST() commands
# that are excluded by CMake control structures, i.e. IF() commands.
ADD_TEST(testlib_assert "./testlib_test_all" "test_assert")
ADD_TEST(testlib_macros "./testlib_test_all" "test_macros")
ADD_TEST(testlib_args "./testlib_test_all" "test_args" "one" "two")
ADD_TEST(testlib_root_dir "./testlib_test_all" "test_root_dir")
ADD_TEST(testlib_test_link "./testlib_test_link")
ADD_TEST(testlib_all "./testlib_test_all" "all" "one" "two")
