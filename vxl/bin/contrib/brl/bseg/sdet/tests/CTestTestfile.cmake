# CMake generated Testfile for 
# Source directory: /home/antiw/demos/vxl-1.13.0/contrib/brl/bseg/sdet/tests
# Build directory: /mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/contrib/brl/bseg/sdet/tests
# 
# This file replicates the SUBDIRS() and ADD_TEST() commands from the source
# tree CMakeLists.txt file, skipping any SUBDIRS() or ADD_TEST() commands
# that are excluded by CMake control structures, i.e. IF() commands.
ADD_TEST(sdet_test_detector "./sdet_test_all" "test_detector" "/home/antiw/demos/vxl-1.13.0/contrib/brl/bseg/sdet/tests/jar-closeup.tif")
ADD_TEST(sdet_test_region_proc "./sdet_test_all" "test_region_proc" "/home/antiw/demos/vxl-1.13.0/contrib/brl/bseg/sdet/tests/jar-closeup.tif")
ADD_TEST(sdet_test_gauss_fit "./sdet_test_all" "test_gauss_fit")
