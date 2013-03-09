# CMake generated Testfile for 
# Source directory: /home/antiw/demos/vxl-1.13.0/contrib/rpl/rgrl/examples
# Build directory: /mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/contrib/rpl/rgrl/examples
# 
# This file replicates the SUBDIRS() and ADD_TEST() commands from the source
# tree CMakeLists.txt file, skipping any SUBDIRS() or ADD_TEST() commands
# that are excluded by CMake control structures, i.e. IF() commands.
ADD_TEST(rgrl_registration1_test "./registration1" "/home/antiw/demos/vxl-1.13.0/contrib/rpl/rgrl/examples/IMG0002.pgm.txt")
ADD_TEST(rgrl_registration2_test "./registration2" "/home/antiw/demos/vxl-1.13.0/contrib/rpl/rgrl/examples/IMG0002.pgm.txt" "/home/antiw/demos/vxl-1.13.0/contrib/rpl/rgrl/examples/IMG0003.pgm.txt")
ADD_TEST(rgrl_registration3_test "./registration3" "/home/antiw/demos/vxl-1.13.0/contrib/rpl/rgrl/examples/IMG0002.pgm.txt" "/home/antiw/demos/vxl-1.13.0/contrib/rpl/rgrl/examples/IMG0002_low.pgm.txt" "/home/antiw/demos/vxl-1.13.0/contrib/rpl/rgrl/examples/IMG0003.pgm.txt" "/home/antiw/demos/vxl-1.13.0/contrib/rpl/rgrl/examples/IMG0003_low.pgm.txt")
ADD_TEST(rgrl_registration_simple_shapes_test "./registration_simple_shapes")
ADD_TEST(rgrl_registration_simple_shapes_outliers_test "./registration_simple_shapes_outliers")
ADD_TEST(rgrl_registration_blackbox "./registration_blackbox")
ADD_TEST(rgrl_registration_landmark_test "./registration_landmark" "/home/antiw/demos/vxl-1.13.0/contrib/rpl/rgrl/examples/IMG0004.pgm.landmarks.txt" "/home/antiw/demos/vxl-1.13.0/contrib/rpl/rgrl/examples/IMG0002.pgm.landmarks.txt" "/home/antiw/demos/vxl-1.13.0/contrib/rpl/rgrl/examples/mask.png")
ADD_TEST(rgrl_registration_retina_no_view_test "./registration_retina_no_view" "/home/antiw/demos/vxl-1.13.0/contrib/rpl/rgrl/examples/IMG0002.pgm.txt" "/home/antiw/demos/vxl-1.13.0/contrib/rpl/rgrl/examples/IMG0002.pgm.landmarks.invariant.txt" "/home/antiw/demos/vxl-1.13.0/contrib/rpl/rgrl/examples/IMG0004.pgm.txt" "/home/antiw/demos/vxl-1.13.0/contrib/rpl/rgrl/examples/IMG0004.pgm.landmarks.invariant.txt" "/home/antiw/demos/vxl-1.13.0/contrib/rpl/rgrl/examples/mask.png")
ADD_TEST(rgrl_registration_range_data_test "./registration_range_data" "/home/antiw/demos/vxl-1.13.0/contrib/rpl/rgrl/examples/bun000.txt.out" "/home/antiw/demos/vxl-1.13.0/contrib/rpl/rgrl/examples/bun045.txt.out")
ADD_TEST(rgrl_registration_blackbox_range_data_test "./registration_range_data_blackbox" "/home/antiw/demos/vxl-1.13.0/contrib/rpl/rgrl/examples/dragonStandRight_0_normals.txt")
ADD_TEST(rgrl_estimate_normals_3d_test "./estimate_normals_3d" "/home/antiw/demos/vxl-1.13.0/contrib/rpl/rgrl/examples/IMG0002.pgm.txt" "IMG0002.pgm.out.txt")
ADD_TEST(rgrl_pseudo_match_2d_test "./match_2d" "1" "/home/antiw/demos/vxl-1.13.0/contrib/rpl/rgrl/examples/sigcube1_face_features.txt" "/home/antiw/demos/vxl-1.13.0/contrib/rpl/rgrl/examples/sigcube1.png" "/home/antiw/demos/vxl-1.13.0/contrib/rpl/rgrl/examples/sigcube2.png" "-init" "/home/antiw/demos/vxl-1.13.0/contrib/rpl/rgrl/examples/init_xform_flipped_y.txt" "-model" "affine")
ADD_TEST(rgrl_pseudo_match_2d_mult_test "./match_2d_multi" "1" "/home/antiw/demos/vxl-1.13.0/contrib/rpl/rgrl/examples/sigcube1_face_features.txt" "/home/antiw/demos/vxl-1.13.0/contrib/rpl/rgrl/examples/sigcube1.png" "/home/antiw/demos/vxl-1.13.0/contrib/rpl/rgrl/examples/sigcube2.png" "-init" "/home/antiw/demos/vxl-1.13.0/contrib/rpl/rgrl/examples/init_xform_flipped_y.txt" "-model" "affine")
