#include <iostream>
#include "oldBlend.h"
#include "blendFuncs.h"
using namespace std;

namespace Adrien {


  void PoissonBlender::blendChannels(unsigned char* img[3], unsigned char* final[3], FILE* fp) {
    _img = img;
    _final = final;
    for (int chnl = 0 ; chnl < 3 ; chnl++) {
      // uncomment this to get the most brain-dead blending
      //mat->brainDeadPaste(img[chnl], final[chnl]);
      //return;

      // create vectors to hold the unknown left hand side (LHS) x of the
      // Poisson equation, and known RHS r of the equation.
      Areal *x = (Areal *) malloc(getNumPixels() * sizeof(Areal));
      Areal *r = (Areal *) malloc(getNumPixels() * sizeof(Areal));
      setRVector(img[chnl], final[chnl], r);
      setXVector(img[chnl], x);

      // solve the system
      Areal epsilon = 0.001; // how low should we go?
      //Areal epsilon = 0.000001; // how low should we go? 
      int steps = 1000;//2 * mat->getNumPixels(); // max steps
      Areal error = ConjGrad(getNumPixels(), this, x, r, epsilon, &steps, fp);
      cout << "completed Poisson blending in " << steps << " steps with error " << error << endl;
      //fprintf(fp, "Completed Poisson blending in %d steps with error %f\n",steps,error);
      if (steps == 5000)
      //fprintf(fp, "WARNING: 5000 steps is maximum. May need to increase number of steps.\n");
	cout << "WARNING: 5000 steps is maximum. May need to increase \"steps\" in blend.cpp" << endl;
      fflush(fp);
      // paste the information into result
      pasteRIntoImg(x, final[chnl]);

      // free up vectors
      free(x);
      free(r);
    }
  }

  /*
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
  */


  PoissonBlender::PoissonBlender(int wInit, int hInit, int feature, ushort *labeling, Areal edgeThres) {
    //int widthInit, int heightInit, int depthInit, tripleBoolGrid *boundariesInit)
  
    // initialize arguments
    w = wInit;
    h = hInit;
    _edgeThres = edgeThres;

    // create lookup tables in each direction
    _pixelToIndex = (int *) malloc(w * h * sizeof(int));
    memset(_pixelToIndex, 255, w * h * sizeof(int)); // set to -1
    for (int x = 0 ; x < w ; x++) {
      for (int y = 0 ; y < h ; y++) {
	if (labeling[imgIndex(x, y, w)] == feature) {
	  _pixelToIndex[imgIndex(x, y, w)] = (int)_indexToPixel.size();
	  _indexToPixel.push_back(Coord(x, y));
	}
      }
    }
  }

  PoissonBlender::~PoissonBlender() {
    free(_pixelToIndex);
  }

  // returns the number of pixels that will be set with this blend operation
  int PoissonBlender::getNumPixels() const {
    return (int)_indexToPixel.size();
  }


  // returns sum of magnitude of edge between guideLoc & finalLoc in guiding and final images
  Areal PoissonBlender::getE(const int gI, const int fI, const bool vertCut) {
    //assert(fabs(guideLoc.x - finalLoc.x) == 1 ^ fabs(guideLoc.y - finalLoc.y) == 1);
    //int gI = imgIndex(guideLoc.x, guideLoc.y, w),
    //fI = imgIndex(finalLoc.x, finalLoc.y, w);

    Areal sumA=0, sumB=0, d;
    for (int c=0; c<3; ++c) {  // iterate over colors

      if (vertCut) { //vertical cut

	d = Areal(_img[c][gI-w]) + 2. * Areal(_img[c][gI]) + Areal(_img[c][gI+w]) -
	  Areal(_img[c][fI-w]) - 2. * Areal(_img[c][fI]) - Areal(_img[c][fI+w]);
	d /= 3.;
	sumA += d*d;

	d = Areal(_final[c][gI-w]) + 2. * Areal(_final[c][gI]) + Areal(_final[c][gI+w]) -
	  Areal(_final[c][fI-w]) - 2. * Areal(_final[c][fI]) - Areal(_final[c][fI+w]);
	d /= 3.;
	sumB += d*d;

      }
      else {   // horizontal cut
	
	d = Areal(_img[c][gI-1]) + 2. * Areal(_img[c][gI]) + Areal(_img[c][gI+1]) -
	  Areal(_img[c][fI-1]) - 2. * Areal(_img[c][fI]) - Areal(_img[c][fI+1]);
	d /= 3.;
	sumA += d*d;
	
	d = Areal(_final[c][gI-1]) + 2. * Areal(_final[c][gI]) + Areal(_final[c][gI+1]) -
	  Areal(_final[c][fI-1]) - 2. * Areal(_final[c][fI]) - Areal(_final[c][fI+1]);
	d /= 3.;
	sumB += d*d;
	
      }
    }

    sumA = sqrt(sumA);
    sumB = sqrt(sumB);
    return sumA + sumB;
  }

  // Tbis matrix will contain the right hand side of the poisson
  // equation.  In general, this is lambda, where del^2 rho = lambda. In
  // this poisson blending case this will be the Laplacian of the
  // underlying image plus boundary terms.
  void PoissonBlender::setRVector(unsigned char *guidingImg, unsigned char *underlyingImg, Areal *r) {
    // Index layout:
    //
    //   b
    // c a d
    //   e

    for (int i = 0 ; i < getNumPixels() ; i++) {
      // indices
      int x = _indexToPixel[i].x, y = _indexToPixel[i].y;
      int a = imgIndex(x, y, w);
      int b = imgIndex(x, y - 1, w);
      int c = imgIndex(x - 1, y, w);
      int d = imgIndex(x + 1, y, w);
      int e = imgIndex(x, y + 1, w);

      // Laplacian of guiding image.
      r[i] = -4.0 * guidingImg[a];
      r[i] += guidingImg[b] + guidingImg[c] + guidingImg[d] + guidingImg[e];

      // Add boundary terms as necessary.

      if (_pixelToIndex[b] < 0) {
	if (getE(a,b,false) < _edgeThres)
	  r[i] -= underlyingImg[b];
	else
	  r[i] -= guidingImg[b];
      }

      if (_pixelToIndex[c] < 0) {
	if (getE(a,c,true) < _edgeThres)
	  r[i] -= underlyingImg[c];
	else
	  r[i] -= guidingImg[c];
      }

      if (_pixelToIndex[d] < 0) {
	if (getE(a,d,true) < _edgeThres)
	  r[i] -= underlyingImg[d];
	else
	  r[i] -= guidingImg[d];
      }

      if (_pixelToIndex[e] < 0) {
	if (getE(a,e,false) < _edgeThres)
	  r[i] -= underlyingImg[e];
	else
	  r[i] -= guidingImg[e];
      }

      /*
      if (_pixelToIndex[b] < 0)
	r[i] -= underlyingImg[b];
      if (_pixelToIndex[c] < 0)
	r[i] -= underlyingImg[c];
      if (_pixelToIndex[d] < 0)
	r[i] -= underlyingImg[d];
      if (_pixelToIndex[e] < 0)
      r[i] -= underlyingImg[e];*/
    }
  }

  // Same as above but gives first guess for unknown, x vecotr
  void PoissonBlender::setXVector(unsigned char *img, Areal *x) {
    for (int i = 0 ; i < getNumPixels() ; i++) {
      x[i] = img[imgIndex(_indexToPixel[i].x, _indexToPixel[i].y, w)];
      //     cout << x[i] << " " << ((int) img[imgIndex(_indexToPixel[i].x, _indexToPixel[i].y, w)]) << endl;
    }
  }


  // This function will paste the r vector into the image by clamping,
  // converting to unsigned char, and then putting the value where
  // approriate in the image.
  void PoissonBlender::pasteRIntoImg(Areal *r, unsigned char *resultImg) {
    for (int i = 0 ; i < getNumPixels() ; i++) {
      //     cout << r[i] << " ";

      // clamp
      int val = (int) r[i];
      if (val < 0)
	val = 0;
      if (val > 255)
	val = 255;

      // now set the pixel
      resultImg[imgIndex(_indexToPixel[i].x, _indexToPixel[i].y, w)] = (unsigned char) val;

      //     cout << ((int) resultImg[imgIndex(_indexToPixel[i].x, _indexToPixel[i].y, w)]) << endl;
    }
  }


  // Used for debugging. Simply sets final to img inside the paste region.
  void PoissonBlender::brainDeadPaste(unsigned char *img, unsigned char* final) {
    int n = w * h;
    for (int i = 0 ; i < n ; i++) {
      if (_pixelToIndex[i] >= 0)
	final[i] = img[i];
    }
  }


  void PoissonBlender::matVecMult(const Areal x[], Areal r[]) const {
    // Index layout:
    //
    //   b
    // c a d
    //   e

    for (int i = 0 ; i < getNumPixels() ; i++) {
      // indices
      int xPxl = _indexToPixel[i].x, yPxl = _indexToPixel[i].y;
      int b = imgIndex(xPxl, yPxl - 1, w);
      int c = imgIndex(xPxl - 1, yPxl, w);
      int d = imgIndex(xPxl + 1, yPxl, w);
      int e = imgIndex(xPxl, yPxl + 1, w);

      // Central term of laplacian.
      r[i] = -4.0 * x[i];

      // Add outer terms if not boundary.
      if (_pixelToIndex[b] >= 0)
	r[i] += x[_pixelToIndex[b]];
      if (_pixelToIndex[c] >= 0)
	r[i] += x[_pixelToIndex[c]];
      if (_pixelToIndex[d] >= 0)
	r[i] += x[_pixelToIndex[d]];
      if (_pixelToIndex[e] >= 0)
	r[i] += x[_pixelToIndex[e]];
    }
  }

}
