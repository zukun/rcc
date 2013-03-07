#ifndef OLDBLEND_H
#define OLDBLEND_H

#include <vector>
#include <string>
#include <stdio.h>

#include "../ImageAbs.h"
#include "../coord.h"
#include "../linearSolver.h"

typedef unsigned short ushort;


namespace Adrien {

  class PoissonBlender : public implicitMatrix {
  public:
    PoissonBlender(int wInit, int hInit, int feature, ushort *labeling, Areal edgeThres);
    virtual ~PoissonBlender();

    // returns the number of pixels that will be set with this blend operation
    int getNumPixels() const;

    // Tbis matrix will contain the right hand side of the poisson
    // equation.  In general, this is lambda, where del^2 rho = lambda. In
    // this poisson blending case this will be the Laplacian of the
    // underlying image plus boundary terms.
    void setRVector(unsigned char *guidingImg, unsigned char *underlyingImg, Areal *r);

    // Same as above but gives first guess for unknown, x vecotr
    void setXVector(unsigned char *img, Areal *x);

    // This function will paste the r vector into the image by clamping, converting
    // to unsigned char, and then putting the value where approriate in the image.
    void pasteRIntoImg(Areal *r, unsigned char *resultImg);

    // Used for debugging. Simply sets final to img inside the paste region.
    void brainDeadPaste(unsigned char *img, unsigned char* final);

    void matVecMult(const Areal x[], Areal r[]) const;
    
    void blendChannels(unsigned char *img[3], unsigned char* final[3], FILE* fp);

  private:
    
    // returns sum of magnitude of edge between guideLoc & finalLoc in guiding and final images
    // (gI, fI are indii of guideLoc, finalLoc), vertCut is true if vertical cut, false otherwise
    Areal getE(const int gI, const int fI, const bool vertCut);

    int w, h;
    Areal _edgeThres;
    std::vector<Coord> _indexToPixel;
    int *_pixelToIndex;
    unsigned char** _img;
    unsigned char** _final;
  };
}

#endif
