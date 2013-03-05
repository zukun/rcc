# Install script for directory: /projects/vision/6/jaechul/flann-1.21-src-64/src/matlab

# Set the install prefix
IF(NOT DEFINED CMAKE_INSTALL_PREFIX)
  SET(CMAKE_INSTALL_PREFIX "/projects/vision/6/jaechul/flann-1.21-src-64/src/../build")
ENDIF(NOT DEFINED CMAKE_INSTALL_PREFIX)
STRING(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
IF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  IF(BUILD_TYPE)
    STRING(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  ELSE(BUILD_TYPE)
    SET(CMAKE_INSTALL_CONFIG_NAME "")
  ENDIF(BUILD_TYPE)
  MESSAGE(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
ENDIF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)

# Set the component getting installed.
IF(NOT CMAKE_INSTALL_COMPONENT)
  IF(COMPONENT)
    MESSAGE(STATUS "Install component: \"${COMPONENT}\"")
    SET(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  ELSE(COMPONENT)
    SET(CMAKE_INSTALL_COMPONENT)
  ENDIF(COMPONENT)
ENDIF(NOT CMAKE_INSTALL_COMPONENT)

# Install shared libraries without execute permission?
IF(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  SET(CMAKE_INSTALL_SO_NO_EXE "1")
ENDIF(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/matlab" TYPE FILE FILES
    "/projects/vision/6/jaechul/flann-1.21-src-64/tmp/matlab/nearest_neighbors.mexa64"
    "/projects/vision/6/jaechul/flann-1.21-src-64/src/matlab/flann_build_index.m"
    "/projects/vision/6/jaechul/flann-1.21-src-64/src/matlab/flann_free_index.m"
    "/projects/vision/6/jaechul/flann-1.21-src-64/src/matlab/flann_search.m"
    "/projects/vision/6/jaechul/flann-1.21-src-64/src/matlab/test_flann.m"
    "/projects/vision/6/jaechul/flann-1.21-src-64/src/matlab/flann_load_index.m"
    "/projects/vision/6/jaechul/flann-1.21-src-64/src/matlab/flann_save_index.m"
    "/projects/vision/6/jaechul/flann-1.21-src-64/src/matlab/mex_compile_script.m"
    "/projects/vision/6/jaechul/flann-1.21-src-64/src/matlab/flann_radius_search.m"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

