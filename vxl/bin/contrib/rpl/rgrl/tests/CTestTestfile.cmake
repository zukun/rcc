# CMake generated Testfile for 
# Source directory: /home/antiw/demos/vxl-1.13.0/contrib/rpl/rgrl/tests
# Build directory: /mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/contrib/rpl/rgrl/tests
# 
# This file replicates the SUBDIRS() and ADD_TEST() commands from the source
# tree CMakeLists.txt file, skipping any SUBDIRS() or ADD_TEST() commands
# that are excluded by CMake control structures, i.e. IF() commands.
ADD_TEST(rgrl_rgrl_util "./rgrl_test_all" "test_rgrl_util")
ADD_TEST(rgrl_transformation "./rgrl_test_all" "test_transformation")
ADD_TEST(rgrl_trans_reader "./rgrl_test_all" "test_trans_reader" "/home/antiw/demos/vxl-1.13.0/contrib/rpl/rgrl/tests/affine_xform.txt")
ADD_TEST(rgrl_feature "./rgrl_test_all" "test_feature")
ADD_TEST(rgrl_feature_region "./rgrl_test_all" "test_feature_region")
ADD_TEST(rgrl_feature_set "./rgrl_test_all" "test_feature_set")
ADD_TEST(rgrl_match_set "./rgrl_test_all" "test_match_set")
ADD_TEST(rgrl_spline "./rgrl_test_all" "test_spline")
ADD_TEST(rgrl_estimator "./rgrl_test_all" "test_estimator")
ADD_TEST(rgrl_convergence "./rgrl_test_all" "test_convergence")
ADD_TEST(rgrl_scale_est "./rgrl_test_all" "test_scale_est")
ADD_TEST(rgrl_initializer "./rgrl_test_all" "test_initializer")
ADD_TEST(rgrl_set_of "./rgrl_test_all" "test_set_of")
ADD_TEST(rgrl_initializer_ran_sam "./rgrl_test_all" "test_initializer_ran_sam")
ADD_TEST(rgrl_matcher "./rgrl_test_all" "test_matcher")
