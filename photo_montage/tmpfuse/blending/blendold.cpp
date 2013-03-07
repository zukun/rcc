#include <iostream>
#include "blend.h"

using namespace std;

void blend(int w, int h, unsigned char* labeling, std::vector<Coord>* displacements, std::vector<unsigned char*> * imgs, unsigned char* final) {
  // Make sure the labeling is zero (backround) along border.
  unsigned char *lab = (unsigned char *) malloc(w * h);
  enforceLabelingContraint(w, h, labeling, lab);
  
  // Allocate some space into which we'll do our computations.
  unsigned char *finlChannels[] = {(unsigned char *) malloc(w * h),
				   (unsigned char *) malloc(w * h),
				   (unsigned char *) malloc(w * h)};
  unsigned char *imagChannels[] = {(unsigned char *) malloc(w * h),
				   (unsigned char *) malloc(w * h),
				   (unsigned char *) malloc(w * h)};
  unsigned char *offsetImg = (unsigned char *) malloc(w * h * 3);

  // Decompose the background image into its color channels.
  // This forms basis for our result.
  channelDecompose(w, h, (*imgs)[0], finlChannels[0], finlChannels[1], finlChannels[2]);  

  // now blend in each feature, in order
  for (unsigned int feature = 1 ; feature < imgs->size() ; feature++) {
    // offset the image
    offsetImageRGB(w, h, (*displacements)[feature], (*imgs)[feature], offsetImg);

    // Decompose this feature image into its color channels
    channelDecompose(w, h, offsetImg, imagChannels[0], imagChannels[1], imagChannels[2]);

    // Create the matrix for this blend;
    PoissonBlender blendMatrix(w, h, feature, lab);
    if (blendMatrix.getNumPixels() == 0) {
      cout << "WARNING: Feature " << feature << " has no values in labeling." << endl;
      continue;
    }

    // now blend in, one channel at a time
    for (int chnl = 0 ; chnl < 3 ; chnl++)
      blendChannel(w, h, &blendMatrix, imagChannels[chnl], finlChannels[chnl]);
  }

  // now put the whole thing back together
  channelCombine(w, h, finlChannels[0], finlChannels[1], finlChannels[2], final);

  // free all allocated memory
  free(lab);
  free(finlChannels[0]);
  free(finlChannels[1]);
  free(finlChannels[2]);
  free(imagChannels[0]);
  free(imagChannels[1]);
  free(imagChannels[2]);
  free(offsetImg);
}

void blendChannel(int w, int h, PoissonBlender *mat, unsigned char *img, unsigned char* final) {
  // uncomment this to get the most brain-dead blending
  //mat->brainDeadPaste(img, final);
  //return;

  // create vectors to hold the unknown left hand side (LHS) x of the
  // Poisson equation, and known RHS r of the equation.
  double *x = (double *) malloc(mat->getNumPixels() * sizeof(double));
  double *r = (double *) malloc(mat->getNumPixels() * sizeof(double));
  mat->setRVector(img, final, r);
  mat->setXVector(img, x);

  // solve the system
  double epsilon = 0.001; // how low should we go?
  int steps = 1000;//2 * mat->getNumPixels(); // max steps
  double error = ConjGrad(mat->getNumPixels(), mat, x, r, epsilon, &steps);
  cout << "completed Poisson blending in " << steps << " steps with error " << error << endl;
  if (steps == 5000)
    cout << "WARNING: 5000 steps is maximum. May need to increase \"steps\" in blend.cpp" << endl;

  // paste the information into result
  mat->pasteRIntoImg(x, final);

  // free up vectors
  free(x);
  free(r);
}

void rgbToRgba(int w, int h, unsigned char *rgbBuffer, unsigned char *rgbaBuffer) {
  int n = w * h;
  for (int i = 0 ; i < n ; i++) {
    rgbaBuffer[4 * i + 0] = rgbBuffer[3 * i + 0];
    rgbaBuffer[4 * i + 1] = rgbBuffer[3 * i + 1];
    rgbaBuffer[4 * i + 2] = rgbBuffer[3 * i + 2];
    rgbaBuffer[4 * i + 3] = 255;
  }
}

void rgbaToRgb(int w, int h, unsigned char *rgbaBuffer, unsigned char *rgbBuffer) {
  int n = w * h;
  for (int i = 0 ; i < n ; i++) {
    rgbBuffer[3 * i + 0] = rgbaBuffer[4 * i + 0]; 
    rgbBuffer[3 * i + 1] = rgbaBuffer[4 * i + 1]; 
    rgbBuffer[3 * i + 2] = rgbaBuffer[4 * i + 2]; 
  }
}

int imgIndex(int x, int y, int w) {
  return y * w + x;
}

int imgIndexRGB(int x, int y, int w) {
  return imgIndex(x, y, w) * 3;
}

void channelDecompose(int w, int h, unsigned char *img, unsigned char *r, unsigned char *g, unsigned char *b) {
  int n = w * h;
  for (int i = 0 ; i < n ; i++) {
    r[i] = img[3 * i + 0];
    g[i] = img[3 * i + 1];
    b[i] = img[3 * i + 2];
  }
}

void channelCombine(int w, int h, unsigned char *r, unsigned char *g, unsigned char *b, unsigned char *img) {
  int n = w * h;
  for (int i = 0 ; i < n ; i++) {
    img[3 * i + 0] = r[i]; 
    img[3 * i + 1] = g[i]; 
    img[3 * i + 2] = b[i]; 
  }
}

// Make sure the labeling is zero (backround) along border.
void enforceLabelingContraint(int w, int h, unsigned char *oldLabeling, unsigned char *result) {
  memcpy(result, oldLabeling, w * h);
  for (int i = 0 ; i < w ; i++)
    result[imgIndex(i, 0, w)] = result[imgIndex(i, h - 1, w)] = 0;
  for (i = 0 ; i < h ; i++)
    result[imgIndex(0, i, w)] = result[imgIndex(w - 1, i, w)] = 0;
}

// pushes the image by coord, and fills black
void offsetImageRGB(int w, int h, Coord offset, unsigned char *in, unsigned char *out) {
  for (int x = 0 ; x < w ; x++) {
    for (int y = 0 ; y < h ; y++) {
      int xPrime = x - offset.x;
      int yPrime = y - offset.y;
      if ((xPrime < 0) || (xPrime >= w)) {
	out[imgIndexRGB(x, y, w) + 0] = 0;
	out[imgIndexRGB(x, y, w) + 1] = 0;
	out[imgIndexRGB(x, y, w) + 2] = 0;
      } else if ((yPrime < 0) || (yPrime >= h)) {
	out[imgIndexRGB(x, y, w) + 0] = 0;
	out[imgIndexRGB(x, y, w) + 1] = 0;
	out[imgIndexRGB(x, y, w) + 2] = 0;
      } else {
	out[imgIndexRGB(x, y, w) + 0] = in[imgIndexRGB(xPrime, yPrime, w) + 0];
	out[imgIndexRGB(x, y, w) + 1] = in[imgIndexRGB(xPrime, yPrime, w) + 1];
	out[imgIndexRGB(x, y, w) + 2] = in[imgIndexRGB(xPrime, yPrime, w) + 2];
      }
    }
  }
}

PoissonBlender::PoissonBlender(int wInit, int hInit, int feature, unsigned char *labeling) {
  //int widthInit, int heightInit, int depthInit, tripleBoolGrid *boundariesInit)
  
  // initialize arguments
  w = wInit;
  h = hInit;

  // create lookup tables in each direction
  pixelToIndex = (int *) malloc(w * h * sizeof(int));
  memset(pixelToIndex, 255, w * h * sizeof(int)); // set to -1
  for (int x = 0 ; x < w ; x++) {
    for (int y = 0 ; y < h ; y++) {
      if (labeling[imgIndex(x, y, w)] == feature) {
	pixelToIndex[imgIndex(x, y, w)] = indexToPixel.size();
	indexToPixel.push_back(Coord(x, y));
      }
    }
  }
}

PoissonBlender::~PoissonBlender() {
  free(pixelToIndex);
}

// returns the number of pixels that will be set with this blend operation
int PoissonBlender::getNumPixels() const {
  return indexToPixel.size();
}

// Tbis matrix will contain the right hand side of the poisson
// equation.  In general, this is lambda, where del^2 rho = lambda. In
// this poisson blending case this will be the Laplacian of the
// underlying image plus boundary terms.
void PoissonBlender::setRVector(unsigned char *guidingImg, unsigned char *underlyingImg, double *r) {
  // Index layout:
  //
  //   b
  // c a d
  //   e

  for (int i = 0 ; i < getNumPixels() ; i++) {
    // indices
    int x = indexToPixel[i].x, y = indexToPixel[i].y;
    int a = imgIndex(x, y, w);
    int b = imgIndex(x, y - 1, w);
    int c = imgIndex(x - 1, y, w);
    int d = imgIndex(x + 1, y, w);
    int e = imgIndex(x, y + 1, w);

    // Laplacian of guiding image.
    r[i] = -4.0 * guidingImg[a];
    r[i] += guidingImg[b] + guidingImg[c] + guidingImg[d] + guidingImg[e];

    // Add boundary terms as necessary.
    if (pixelToIndex[b] < 0)
      r[i] -= underlyingImg[b];
    if (pixelToIndex[c] < 0)
      r[i] -= underlyingImg[c];
    if (pixelToIndex[d] < 0)
      r[i] -= underlyingImg[d];
    if (pixelToIndex[e] < 0)
      r[i] -= underlyingImg[e];
  }
}

// Same as above but gives first guess for unknown, x vecotr
void PoissonBlender::setXVector(unsigned char *img, double *x) {
  for (int i = 0 ; i < getNumPixels() ; i++) {
    x[i] = img[imgIndex(indexToPixel[i].x, indexToPixel[i].y, w)];
//     cout << x[i] << " " << ((int) img[imgIndex(indexToPixel[i].x, indexToPixel[i].y, w)]) << endl;
  }
}


// This function will paste the r vector into the image by clamping,
// converting to unsigned char, and then putting the value where
// approriate in the image.
void PoissonBlender::pasteRIntoImg(double *r, unsigned char *resultImg) {
  for (int i = 0 ; i < getNumPixels() ; i++) {
//     cout << r[i] << " ";

    // clamp
    int val = (int) r[i];
    if (val < 0)
      val = 0;
    if (val > 255)
      val = 255;

    // now set the pixel
    resultImg[imgIndex(indexToPixel[i].x, indexToPixel[i].y, w)] = (unsigned char) val;

//     cout << ((int) resultImg[imgIndex(indexToPixel[i].x, indexToPixel[i].y, w)]) << endl;
  }
}


// Used for debugging. Simply sets final to img inside the paste region.
void PoissonBlender::brainDeadPaste(unsigned char *img, unsigned char* final) {
  int n = w * h;
  for (int i = 0 ; i < n ; i++) {
    if (pixelToIndex[i] >= 0)
      final[i] = img[i];
  }
}


void PoissonBlender::matVecMult(const double x[], double r[]) const {
  // Index layout:
  //
  //   b
  // c a d
  //   e

  for (int i = 0 ; i < getNumPixels() ; i++) {
    // indices
    int xPxl = indexToPixel[i].x, yPxl = indexToPixel[i].y;
    int b = imgIndex(xPxl, yPxl - 1, w);
    int c = imgIndex(xPxl - 1, yPxl, w);
    int d = imgIndex(xPxl + 1, yPxl, w);
    int e = imgIndex(xPxl, yPxl + 1, w);

    // Central term of laplacian.
    r[i] = -4.0 * x[i];

    // Add outer terms if not boundary.
    if (pixelToIndex[b] >= 0)
      r[i] += x[pixelToIndex[b]];
    if (pixelToIndex[c] >= 0)
      r[i] += x[pixelToIndex[c]];
    if (pixelToIndex[d] >= 0)
      r[i] += x[pixelToIndex[d]];
    if (pixelToIndex[e] >= 0)
      r[i] += x[pixelToIndex[e]];
  }
}
