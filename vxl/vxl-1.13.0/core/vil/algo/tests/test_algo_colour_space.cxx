// This is core/vil/algo/tests/test_algo_colour_space.cxx
#include <testlib/testlib_test.h>
#include <vil/algo/vil_colour_space.h>
// not used? #include <vcl_iostream.h>
#include <vcl_cmath.h>

static void test_algo_colour_space()
{
  double red_rgb[3]   = {1.0, 0.0, 0.0};
  double green_rgb[3] = {0.0, 1.0, 0.0};
  double blue_rgb[3]  = {0.0, 0.0, 1.0};

  double color1[3], color2[3];
  // test on red
  vil_colour_space_RGB_to_YUV(red_rgb, color1);
  vil_colour_space_YUV_to_RGB(color1, color2);
  TEST("Test RGB/YUV on red",
       vcl_abs(red_rgb[0]-color2[0])<1e-12 &&
       vcl_abs(red_rgb[1]-color2[1])<1e-12 &&
       vcl_abs(red_rgb[2]-color2[2])<1e-12, true);

  // test on green
  vil_colour_space_RGB_to_YUV(green_rgb, color1);
  vil_colour_space_YUV_to_RGB(color1, color2);
  TEST("Test RGB/YUV on green",
       vcl_abs(green_rgb[0]-color2[0])<1e-12 &&
       vcl_abs(green_rgb[1]-color2[1])<1e-12 &&
       vcl_abs(green_rgb[2]-color2[2])<1e-12, true);

  // test on blue
  vil_colour_space_RGB_to_YUV(blue_rgb, color1);
  vil_colour_space_YUV_to_RGB(color1, color2);
  TEST("Test RGB/YUV on blue",
       vcl_abs(blue_rgb[0]-color2[0])<1e-12 &&
       vcl_abs(blue_rgb[1]-color2[1])<1e-12 &&
       vcl_abs(blue_rgb[2]-color2[2])<1e-12, true);


  //// Test YPbPr ////

  // test on red
  vil_colour_space_RGB_to_YPbPr_601(red_rgb, color1);
  vil_colour_space_YPbPr_601_to_RGB(color1, color2);
  TEST("Test RGB/YPbPr on red",
       vcl_abs(red_rgb[0]-color2[0])<1e-6 &&
       vcl_abs(red_rgb[1]-color2[1])<1e-6 &&
       vcl_abs(red_rgb[2]-color2[2])<1e-6, true);

  // test on green
  vil_colour_space_RGB_to_YPbPr_601(green_rgb, color1);
  vil_colour_space_YPbPr_601_to_RGB(color1, color2);
  TEST("Test RGB/YPbPr on green",
       vcl_abs(green_rgb[0]-color2[0])<1e-6 &&
       vcl_abs(green_rgb[1]-color2[1])<1e-6 &&
       vcl_abs(green_rgb[2]-color2[2])<1e-6, true);

  // test on blue
  vil_colour_space_RGB_to_YPbPr_601(blue_rgb, color1);
  vil_colour_space_YPbPr_601_to_RGB(color1, color2);
  TEST("Test RGB/YPbPr on blue",
       vcl_abs(blue_rgb[0]-color2[0])<1e-6 &&
       vcl_abs(blue_rgb[1]-color2[1])<1e-6 &&
       vcl_abs(blue_rgb[2]-color2[2])<1e-6, true);


  //// Test YCbCr ////

  unsigned char unsigned_red_rgb[3]   = {255, 0,   0  };
  unsigned char unsigned_green_rgb[3] = {0,   255, 0  };
  unsigned char unsigned_blue_rgb[3]  = {0,   0,   255};

  unsigned char unsigned_color1[3], unsigned_color2[3];

  // test on red
  vil_colour_space_RGB_to_YCbCr_601(unsigned_red_rgb, unsigned_color1);
  vil_colour_space_YCbCr_601_to_RGB(unsigned_color1, unsigned_color2);
  TEST("Test RGB/YCbCr on red",
       unsigned_red_rgb[0]-unsigned_color2[0]<1 &&
       unsigned_red_rgb[1]-unsigned_color2[1]<1 &&
       unsigned_red_rgb[2]-unsigned_color2[2]<1, true);

  // test on green
  vil_colour_space_RGB_to_YCbCr_601(unsigned_green_rgb, unsigned_color1);
  vil_colour_space_YCbCr_601_to_RGB(unsigned_color1, unsigned_color2);
  TEST("Test RGB/YCbCr on green",
       unsigned_green_rgb[0]-unsigned_color2[0]<1 &&
       unsigned_green_rgb[1]-unsigned_color2[1]<1 &&
       unsigned_green_rgb[2]-unsigned_color2[2]<1, true);

  // test on blue
  vil_colour_space_RGB_to_YCbCr_601(unsigned_blue_rgb, unsigned_color1);
  vil_colour_space_YCbCr_601_to_RGB(unsigned_color1, unsigned_color2);
  TEST("Test RGB/YCbCr on blue",
       unsigned_blue_rgb[0]-unsigned_color2[0]<1 &&
       unsigned_blue_rgb[1]-unsigned_color2[1]<1 &&
       unsigned_blue_rgb[2]-unsigned_color2[2]<1, true);
}

TESTMAIN(test_algo_colour_space);
