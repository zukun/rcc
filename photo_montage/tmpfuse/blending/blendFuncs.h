#ifndef BLENDFUNCS_H
#define BLENDFUNCS_H

#include <vector>
#include <string>
#include <stdio.h>

#include "../ImageAbs.h"
#include "../coord.h"
#include "../linearSolver.h"
#include "blendFuncs.h"
#include "oldBlend.h"

typedef unsigned short ushort;

namespace Adrien {

  // labeling: w*h of unsigned shorts.  If you have n images (imgs is of length n), then each value of 
  // labeling can be 0->n-1.  
  // final: w*h*3 of unsigned chars (you need to allocate), result goes here.
  // edgeThres: pass in 1000.
  // fp: can pass in NULL, or stdout
  void blend(int w, int h, ushort* labeling, std::vector<ImageAbs*> * imgs, unsigned char* final, 
	     Areal edgeThres, FILE* fp); 


  //void rgbToRgba(int w, int h, unsigned char *rgbBuffer, unsigned char *rgbaBuffer);
  //void rgbaToRgb(int w, int h, unsigned char *rgbaBuffer, unsigned char *rgbBuffer);

  int imgIndex(int x, int y, int w);
  int imgIndexRGB(int x, int y, int w);

  void channelDecompose(int w, int h, unsigned char *img, unsigned char *r, unsigned char *g, unsigned char *b);
  void channelCombine(int w, int h, unsigned char *r, unsigned char *g, unsigned char *b, unsigned char *img);

  // Make sure the labeling is zero (backround) along border.
  void enforceLabelingContraint(int w, int h, ushort *oldLabeling, ushort *result);

  // pushes the image by coord, and fills black
  void offsetImageRGB(int w, int h, ImageAbs* in, unsigned char *out);

}


#endif
