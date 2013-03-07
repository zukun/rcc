#ifndef BLEND_H
#define BLEND_H

#include <vector>
#include <string>

#include "../coord.h"
//#include "imageio/imageio.h"
#include "linearSolver.h"

// ***** Classes ***** //

class PoissonBlender : public implicitMatrix {
 public:
  PoissonBlender(int wInit, int hInit, int feature, unsigned char *labeling);
  virtual ~PoissonBlender();

  // returns the number of pixels that will be set with this blend operation
  int getNumPixels() const;

  // Tbis matrix will contain the right hand side of the poisson
  // equation.  In general, this is lambda, where del^2 rho = lambda. In
  // this poisson blending case this will be the Laplacian of the
  // underlying image plus boundary terms.
  void setRVector(unsigned char *guidingImg, unsigned char *underlyingImg, double *r);

  // Same as above but gives first guess for unknown, x vecotr
  void setXVector(unsigned char *img, double *x);

  // This function will paste the r vector into the image by clamping, converting
  // to unsigned char, and then putting the value where approriate in the image.
  void pasteRIntoImg(double *r, unsigned char *resultImg);

  // Used for debugging. Simply sets final to img inside the paste region.
  void brainDeadPaste(unsigned char *img, unsigned char* final);

  void matVecMult(const double x[], double r[]) const;

 private:
  int w, h;
  std::vector<Coord> indexToPixel;
  int *pixelToIndex;
};

// ***** Functions ***** //

void blend(int width, int height, unsigned char* labeling, std::vector<Coord>* displacements, std::vector<unsigned char*> * imgs, unsigned char* final);

void blendChannel(int w, int h, PoissonBlender *mat, unsigned char *img, unsigned char* final);

void rgbToRgba(int w, int h, unsigned char *rgbBuffer, unsigned char *rgbaBuffer);
void rgbaToRgb(int w, int h, unsigned char *rgbaBuffer, unsigned char *rgbBuffer);

int imgIndex(int x, int y, int w);
int imgIndexRGB(int x, int y, int w);

void channelDecompose(int w, int h, unsigned char *img, unsigned char *r, unsigned char *g, unsigned char *b);
void channelCombine(int w, int h, unsigned char *r, unsigned char *g, unsigned char *b, unsigned char *img);

// Make sure the labeling is zero (backround) along border.
void enforceLabelingContraint(int w, int h, unsigned char *oldLabeling, unsigned char *result);

// pushes the image by coord, and fills black
void offsetImageRGB(int w, int h, Coord offset, unsigned char *in, unsigned char *out);

#endif
