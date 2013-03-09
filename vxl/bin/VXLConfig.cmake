# vxl/config/cmake/VXLConfig.cmake.in
#   also configured by CMake to
# /mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/VXLConfig.cmake
#
# This CMake module is configured by VXL's build process to export the
# project settings for use by client projects.  A client project may
# find VXL and include this module using the FIND_PACKAGE command:
#
#  FIND_PACKAGE(VXL)
#
# After this command executes, projects may test VXL_FOUND for whether
# VXL has been found.  If so, the settings listed below in this file
# have been loaded and are available for use.
#
# Typically, a client project will include UseVXL.cmake from the
# directory specified by the VXL_CMAKE_DIR setting:
#
#  FIND_PACKAGE(VXL)
#  IF(VXL_FOUND)
#    INCLUDE(${VXL_CMAKE_DIR}/UseVXL.cmake)
#  ELSE(VXL_FOUND)
#    MESSAGE("VXL_DIR should be set to the VXL build directory.")
#  ENDIF(VXL_FOUND)
#
# See vxl/config/cmake/UseVXL.cmake for details.
#

# The build settings file.
SET(VXL_BUILD_SETTINGS_FILE "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/VXLBuildSettings.cmake")

# The VXL library dependencies.
IF(NOT VXL_NO_LIBRARY_DEPENDS)
  INCLUDE("/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/VXLLibraryDepends.cmake")
ENDIF(NOT VXL_NO_LIBRARY_DEPENDS)

# The VXL library directory.
SET(VXL_LIBRARY_DIR "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/lib")

# The VXL CMake support directory.
# Clients projects should not use the Find*.cmake files in this directory.
SET(VXL_CMAKE_DIR "/home/antiw/demos/vxl-1.13.0/config/cmake/Modules")

# VXL Configuration options. You don't have to build with the same options as VXL, but it often helps.
SET(VXL_BUILD_SHARED_LIBS "OFF")
SET(VXL_BUILD_TESTS "")
SET(VXL_BUILD_EXAMPLES "ON")
SET(VXL_EXTRA_CMAKE_CXX_FLAGS "")
SET(VXL_EXTRA_CMAKE_C_FLAGS "")
SET(VXL_EXTRA_CMAKE_EXE_LINKER_FLAGS "")
SET(VXL_EXTRA_CMAKE_MODULE_LINKER_FLAGS "")
SET(VXL_EXTRA_CMAKE_SHARED_LINKER_FLAGS "")

# VXL has many parts that are optional, depending on selections made
# when building.  The stanzas below give a consistent (though
# pedantic) interface to each part.  Clients use these settings to
# determine whether a part was built (_FOUND), where its headers are
# located (_INCLUDE_DIR) and in some cases what libraries must be
# linked to use the part (_LIBRARIES).  Most client projects will
# likely still refer to individual VXL libraries such as vcl, for
# example, by hard-wired "vcl" instead of using the variable
# VXL_VCL_LIBRARIES, but it is there just in case.

SET(VXL_VCL_FOUND "YES" ) # VXL vcl is always FOUND.  It is not optional.
SET(VXL_VCL_INCLUDE_DIR "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/vcl;/home/antiw/demos/vxl-1.13.0/vcl")
SET(VXL_VCL_LIBRARIES "vcl")

SET(VXL_CORE_FOUND "YES" ) # VXL core is always FOUND.  It is not optional.
SET(VXL_CORE_INCLUDE_DIR "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/core;/home/antiw/demos/vxl-1.13.0/core")
# VXL core has many libraries

SET(VXL_CORE_VIDEO_FOUND "YES" )
SET(VXL_CORE_VIDEO_INCLUDE_DIR "/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/core;/home/antiw/demos/vxl-1.13.0/core")
SET(VXL_CORE_VIDEO_LIBRARIES "vidl1 vidl_vil1 vidl")

SET(VXL_VGUI_FOUND "NO")
SET(VXL_VGUI_INCLUDE_DIR "OPENGL_INCLUDE_DIR-NOTFOUND")
SET(VXL_VGUI_LIBRARIES "vgui")

SET(VXL_VGUI_WX_FOUND "")

SET(VXL_CONTRIB_FOUND "ON")
# VXL contrib has subdirectories handled independently below
# VXL contrib has many libraries

SET(VXL_BRL_FOUND "ON")
SET(VXL_BRL_INCLUDE_DIR "/home/antiw/demos/vxl-1.13.0/contrib/brl/bbas;/mnt/data/documents/Computer Vision/Computer Vision Courses - Lectures/Li Fei Fei - Recognizing and Learning Object Categories - ICCV2009/Demo of bag-of-word classifiers/bag_words_demo/bin/contrib/brl/bbas;/home/antiw/demos/vxl-1.13.0/contrib/brl/bseg;/home/antiw/demos/vxl-1.13.0/contrib/brl/bmvl;/home/antiw/demos/vxl-1.13.0/contrib/brl/bpro;/home/antiw/demos/vxl-1.13.0/contrib/brl")
# VXL BRL has many libraries

SET(VXL_BGUI3D_FOUND "")
SET(VXL_BGUI3D_INCLUDE_DIR "")

SET(VXL_COIN3D_FOUND "")
SET(VXL_COIN3D_INCLUDE_DIR "")

SET(VXL_GEL_FOUND "ON")
SET(VXL_GEL_INCLUDE_DIR "/home/antiw/demos/vxl-1.13.0/contrib/gel")
# VXL GEL has many libraries

SET(VXL_MUL_FOUND "ON")
SET(VXL_MUL_INCLUDE_DIR "/home/antiw/demos/vxl-1.13.0/contrib/mul")
# VXL MUL has many libraries

SET(VXL_OUL_FOUND "ON")
SET(VXL_OUL_INCLUDE_DIR "/home/antiw/demos/vxl-1.13.0/contrib/oul")
# VXL OUL has many libraries

SET(VXL_OXL_FOUND "ON")
SET(VXL_OXL_INCLUDE_DIR "/home/antiw/demos/vxl-1.13.0/contrib/oxl")
# VXL OXL has many libraries

SET(VXL_RPL_FOUND "ON")
SET(VXL_RPL_RGTL_FOUND "OFF")
SET(VXL_RPL_INCLUDE_DIR "/home/antiw/demos/vxl-1.13.0/contrib/rpl")
# VXL RPL has many libraries

SET(VXL_TBL_FOUND "ON")
SET(VXL_TBL_INCLUDE_DIR "/home/antiw/demos/vxl-1.13.0/contrib/tbl")
# VXL TBL has many libraries

SET(VXL_CONVERSIONS_FOUND "ON")
SET(VXL_CONVERSIONS_INCLUDE_DIR "/home/antiw/demos/vxl-1.13.0/contrib/conversions")
# VXL CONVERSIONS has no libraries

SET(VXL_TARGETJR_FOUND "OFF")

# Client projects use these setting to find and use the 3rd party
# libraries that VXL either found on the system or built for itself.
# Sometimes, VXL will point client projects to the library VXL built
# for itself, and sometimes VXL will point client projects to the
# system library it found.

SET(VXL_NETLIB_FOUND "YES")
SET(VXL_NETLIB_INCLUDE_DIR "/home/antiw/demos/vxl-1.13.0/v3p/netlib")
SET(VXL_NETLIB_LIBRARIES "netlib;v3p_netlib")

SET(VXL_QV_FOUND "YES")
SET(VXL_QV_INCLUDE_DIR "/home/antiw/demos/vxl-1.13.0/v3p")
SET(VXL_QV_LIBRARIES "Qv")

SET(VXL_ZLIB_FOUND "TRUE")
SET(VXL_ZLIB_INCLUDE_DIR "/usr/include")
SET(VXL_ZLIB_LIBRARIES "/usr/lib/libz.so")

SET(VXL_PNG_FOUND "TRUE")
SET(VXL_PNG_INCLUDE_DIR "/usr/include;/usr/include")
SET(VXL_PNG_LIBRARIES "/usr/lib/libpng.so;/usr/lib/libz.so")

SET(VXL_JPEG_FOUND "TRUE")
SET(VXL_JPEG_INCLUDE_DIR "/usr/include")
SET(VXL_JPEG_LIBRARIES "/usr/lib/libjpeg.so")

SET(VXL_TIFF_FOUND "TRUE")
SET(VXL_TIFF_INCLUDE_DIR "/usr/include")
SET(VXL_TIFF_LIBRARIES "/usr/lib/libtiff.so")

SET(VXL_GEOTIFF_FOUND "YES")
SET(VXL_GEOTIFF_INCLUDE_DIR "/home/antiw/demos/vxl-1.13.0/v3p/geotiff")
SET(VXL_GEOTIFF_LIBRARIES "geotiff")

SET(VXL_MPEG2_FOUND "YES")
SET(VXL_MPEG2_INCLUDE_DIR "/home/antiw/demos/vxl-1.13.0/v3p/mpeg2/include;/home/antiw/demos/vxl-1.13.0/v3p/mpeg2/include/mpeg2dec")
SET(VXL_MPEG2_LIBRARIES "mpeg2;vo")

SET(VXL_RPLY_FOUND "")
SET(VXL_RPLY_INCLUDE_DIR "")
SET(VXL_RPLY_LIBRARIES "")

SET(VXL_COIN3D_FOUND "")
SET(VXL_COIN3D_INCLUDE_DIR "")
SET(VXL_COIN3D_LIBRARY "")

SET(VXL_PYTHON_FOUND "YES")
SET(VXL_PYTHON_INCLUDE_PATH "/usr/include/python2.6")
SET(VXL_PYTHON_PC_INCLUDE_PATH "")
SET(VXL_PYTHON_LIBRARY "/usr/lib/python2.6/config/libpython2.6.so")
SET(VXL_PYTHON_DEBUG_LIBRARY "")

SET(VXL_EXPAT_FOUND "TRUE")
SET(VXL_EXPAT_INCLUDE_DIR "/usr/include")
SET(VXL_EXPAT_LIBRARIES "/usr/lib/libexpat.so")

# Tell UseVXL.cmake that VXLConfig.cmake has been included.
SET(VXL_CONFIG_CMAKE 1)

### deprecated variables set below ###
# These should be removed after some time to upgrade has passed.

# VXL include directories.
# There is no need for clients to use this directly
SET(VXL_V3P_INCLUDE_DIR_deprecated "/home/antiw/demos/vxl-1.13.0/v3p")

# Would a client project need to use these variables ever?
# These are needed for backward compatibility mode for now, see UseVXL.cmake
SET(VXL_VGUI_USE_GLUT_deprecated "")
SET(VXL_VGUI_USE_QT_deprecated "")
SET(VXL_VGUI_USE_MFC_deprecated "")
SET(VXL_VGUI_USE_GTK_deprecated "")
SET(VXL_VGUI_USE_GTK2_deprecated "")

# There is no need for clients to know this.
# These are needed for backward compatibility mode for now, see UseVXL.cmake
SET(VXL_FORCE_V3P_ZLIB_deprecated "OFF")
SET(VXL_FORCE_V3P_JPEG_deprecated "OFF")
SET(VXL_FORCE_V3P_TIFF_deprecated "OFF")
SET(VXL_FORCE_V3P_GEOTIFF_deprecated "OFF")
SET(VXL_FORCE_V3P_PNG_deprecated "OFF")
SET(VXL_FORCE_V3P_MPEG2_deprecated "OFF")
SET(VXL_FORCE_V3P_RPLY_deprecated "")

